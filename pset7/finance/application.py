from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # check to see how much cash user has
    stocks = db.execute("SELECT * FROM portfolios WHERE user_id=:user_id", user_id=session["user_id"])
    user = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])

    if not stocks:
        return apology("User currently owns no stock")

    # prepare to get total value of portfolio
    cash = usd(user[0]["cash"])
    sum = user[0]["cash"]

    # loop through portfolio lines and add missing info via lookup
    for stock in stocks:
        quote = lookup(stock["stock_symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = usd(float(stock["price"]*stock["shares"]))
        sum += float(stock["price"]*stock["shares"])
        stock["price"] = usd(stock["price"])

    # convert total value to USD format
    sum = usd(sum)

    return render_template("index.html", stocks=stocks, cash=cash, total=sum)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reach route via POST
    if request.method == "POST":
        # convert shares to int
        shares = int(request.form.get("shares"))

        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # ensure password was submitted
        elif (not request.form.get("shares")) or shares < 1:
            return apology("must provide positive number of shares to buy")

        # lookup quote based on returned value
        quote = lookup(request.form.get("symbol"))

        # check to make sure symbol exists
        if not quote:
            return apology("that stock symbol couldn't be found")

        # check to see how much cash user has
        cash = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])

        # create math-friendly variables
        user_cash = float(cash[0]["cash"])
        price = float(quote["price"])
        user_id = session["user_id"]

        # return error if user can't afford purchase
        if user_cash < shares * price:
            return apology("You can't afford that many shares")

        # insert stock into portfolio and transaction tables
        current_stock = db.execute("SELECT shares FROM portfolios WHERE user_id=:id AND stock_symbol=:symbol", id=session["user_id"], symbol=request.form.get("symbol"))
        if len(current_stock) == 0:
            result = db.execute("INSERT INTO portfolios (user_id, stock_symbol, shares) VALUES (:user_id, :stock_symbol, :shares)", user_id=session["user_id"], stock_symbol=quote["symbol"], shares=shares)
        else:
            result = db.execute("UPDATE portfolios SET shares=:user_stock WHERE user_id=:id AND stock_symbol=:symbol", user_stock=current_stock[0]["share"]+shares, id=user_id, symbol=request.form.get("symbol"))

        # return error of insert fails
        if not result:
            return apology("Sorry", "It didn't work")

        # insert stock into portfolio and transaction tables
        result = db.execute("INSERT INTO transactions (user_id, stock_symbol, transaction_type, shares, price) VALUES (:user_id, :stock_symbol, :transaction, :shares, :price)", user_id=session["user_id"], stock_symbol=quote["symbol"], shares=shares, transaction="BUY", price=price)

        # return error of insert fails
        if not result:
            return apology("Sorry", "It didn't work")

        # deduct price from user's cash
        db.execute("UPDATE users SET cash=:user_cash WHERE id=:id", user_cash=(user_cash-(shares*price)), id=user_id)

        # redirect to index after purchase
        return redirect(url_for("index"))

    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # get portfolios and transactions
    transactions = db.execute("SELECT * FROM transactions WHERE user_id=:user_id", user_id=session["user_id"])

    if not transactions:
        return apology("No transactions found")

    # loop through transaction lines and add missing info via lookup
    for line in transactions:
        quote = lookup(line["stock_symbol"])
        line["name"] = quote["name"]
        # convert price value to USD format
        line["price"] = usd(line["price"])

    return render_template("history.html", transactions=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if user reached route via POST
    if request.method == "POST":

        # lookup quote based on returned value
        quote = lookup(request.form.get("symbol"))

        # check to make sure quote exists
        if not quote:
            return apology("that stock symbol couldn't be found")


        # render quote data in page
        return render_template("quoted.html", quote=quote)

    # if user reached route via GET, render search form
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        elif request.form.get("password") != request.form.get("confirm-password"):
            return apology("passwords don't match")

        # insert new user into database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=pwd_context.hash(request.form.get("password")))
        # return apology if username already exists
        if not result:
            return apology("username already exists","try a different one")

        # remember which user has logged in
        session["user_id"] = result

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # if user reach route via POST
    if request.method == "POST":
        # convert shares to int
        shares = int(request.form.get("shares"))

        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol")

        # ensure password was submitted
        elif (not request.form.get("shares")) or shares < 1:
            return apology("must provide positive number of shares to sell")

        # lookup quote based on returned value
        quote = lookup(request.form.get("symbol"))

        # check to make sure symbol exists
        if not quote:
            return apology("that stock symbol couldn't be found")

        # find amount of stock currently owned
        stock = db.execute("SELECT shares FROM portfolios WHERE stock_symbol=:symbol", symbol=request.form.get("symbol"))
        user_stock = stock[0]["shares"]

        #check to make sure user owns enough stock to sell
        if not user_stock or user_stock < shares:
            return apology("you don't enough shares for this sale")

        # check to see how much cash user has
        cash = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])

        # create math-friendly variables
        user_cash = float(cash[0]["cash"])
        price = float(quote["price"])
        user_id = session["user_id"]


        # insert stock into transaction tables
        result = db.execute("INSERT INTO transactions (user_id, stock_symbol, transaction_type, shares, price) VALUES (:user_id, :stock_symbol, :transaction, :shares, :price)", user_id=session["user_id"], stock_symbol=quote["symbol"], shares=shares, transaction="SELL", price=price)

        # return error of insert fails
        if not result:
            return apology("Sorry", "It didn't work")

        # add sale price to user's cash
        db.execute("UPDATE users SET cash=:user_cash WHERE id=:id", user_cash=(user_cash+(shares*price)), id=user_id)

        # deduct shares from user's portfolio
        user_stock -= shares
        if user_stock == 0:
            db.execute("DELETE FROM portfolios WHERE user_id=:id AND stock_symbol=:symbol", id=user_id, symbol=request.form.get("symbol"))
        else:
            db.execute("UPDATE portfolios SET shares=:user_stock WHERE user_id=:id AND stock_symbol=:symbol", user_stock=user_stock, id=user_id, symbol=request.form.get("symbol"))

        # redirect to index after purchase
        return redirect(url_for("index"))

    else:
        return render_template("sell.html")
