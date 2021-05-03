import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get user
    user = session["user_id"]

    # Query infos from database
    rows = db.execute("SELECT * FROM stocks WHERE user_id = :user", user=user)
    cash = db.execute("SELECT cash FROM users WHERE id = :user", user=user)[0]['cash']
    total_cash = cash

    # Populate stocks list wit hstock data
    stocks = []
    for index, row in enumerate(rows):
        stock_data = lookup(row['symbol'])
        stock_data['amount'] = row['amount']
        stock_data['quantity'] = round(stock_data['price'] * stock_data['amount'], 2)

        # Generate index table data
        stocks.append(list((
            stock_data['symbol'],
            stock_data['name'],
            stock_data['amount'],
            stock_data['price'],
            stock_data['quantity']
        )))
        total_cash += stocks[index][4]

    return render_template("index.html", stocks=stocks, cash=round(cash, 2), total=round(total_cash, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Defaul to the template
    if request.method == "GET":
        return render_template("buy.html")

    # Hoisting
    amount=int(request.form.get("amount"))
    symbol=lookup(request.form.get("symbol"))['symbol']
    user = session["user_id"]

    # Check validity
    if not lookup(symbol):
        return render_template("buy.html", error=True, messate="This is not a valid stock")

    # Check the amount to be spent
    price=lookup(symbol)['price']
    cash = db.execute("SELECT cash FROM users WHERE id = :user", user=user)[0]['cash']
    wallet = cash - price * float(amount)

    # Check the wallet
    if wallet < 0:
        return render_template("buy.html", error=True, message="Sorry, there is not enough money to complete this operation")

    # Check the current amount of the stock
    stock = db.execute("SELECT amount FROM stocks WHERE user_id = :user AND symbol = :symbol", user=user, symbol=symbol)
    if not stock:
        db.execute("INSERT INTO stocks(user_id, symbol, amount) VALUES (:user, :symbol, :amount)", user=user, symbol=symbol, amount=amount)
    else:
        amount += stock[0]['amount']
        db.execute("UPDATE stocks SET amount = :amount WHERE user_id = :user AND symbol = :symbol", user=user, symbol=symbol, amount=amount)

    # Update wallet
    db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash=wallet, user=user)
    db.execute("INSERT INTO transactions(user_id, symbol, amount, value) VALUES (:user, :symbol, :amount, :value)",
            user=user, symbol=symbol, amount=amount, value=round(price * float(amount)))

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user = session["user_id"]
    rows = db.execute("SELECT * FROM transactions WHERE user_id = :user", user=user)

    # transactions list
    transactions = []
    for row in rows:
        stock_data = lookup(row['symbol'])
        transactions.append(list((
            stock_data['symbol'],
            stock_data['name'],
            row['amount'],
            row['value'],
            row['date'],
        )))

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    if request.method == "GET":
        return render_template("login.html")

    # Ensure username was submitted
    if not request.form.get("username"):
        return render_template("login.html", error=True, message="must provide username")

    # Ensure password was submitted
    elif not request.form.get("password"):
        return render_template("login.html", error=True, message="must provide password")

    # Query database for username
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                      username=request.form.get("username"))

    # Ensure username exists and password is correct
    if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
        return render_template("login.html", error=True, message="invalid username and/or password")

    # Remember which user has logged in
    session["user_id"] = rows[0]["id"]

    # Redirect user to home page
    return redirect("/")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # Default user landing
    if request.method == "GET":
        return render_template("quote.html", stock="")

    #When POST
    stock = lookup(request.form.get("symbol"))

    # If no stock
    if not stock:
        return render_template("quote.html", error=True, message="Stock Unavaible")

    return render_template("quote.html", stock=stock)


@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    """Profile informations."""

    user = session["user_id"]
    rows = db.execute("SELECT * FROM users WHERE id = :user", user=user)

    # Some issues happening
    if not rows:
        return apology("Something wrong with this request", 403)

    userdata = rows[0]

    # Default user landing
    if request.method == "GET":
        return render_template("profile.html", data=userdata)

    # When POST
    # Update user
    username = request.form.get("username")
    password = request.form.get("password")
    confirm_password = request.form.get("confirm-password")

    if not confirm_password:
        return render_template("profile.html", data=userdata, error=True, message="Please confirm your password")

    if password != confirm_password:
        return render_template("profile.html", data=userdata, error=True, message="Passwords don't match")

    # Update username only if different
    if len(username) and username != userdata["username"]:
        db.execute("UPDATE users SET username = :username WHERE id = :user", username=username, user=user)

    # Update password only if different
    if len(password) and not check_password_hash(rows[0]["hash"], request.form.get("password")):
        hashed_password = generate_password_hash(password)
        db.execute("UPDATE users SET hash = :hashed_password WHERE id = :user", hashed_password=hashed_password, user=user)

    return render_template("profile.html", data=userdata, message="Profile updated")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Cear Session before all
    session.clear()

    # Go to register in case of GET
    if request.method == "GET":
        return render_template("register.html")

    # Get inserted details
    username = request.form.get("username")
    password = request.form.get("password")
    confirm_password = request.form.get("confirm-password")

    # Check Empty Username
    if not username:
        return render_template("register.html", error=True, message="ERROR: Please, insert a username")

    # Check Empty Password
    elif not password:
        return render_template("register.html", error=True, message="ERROR: Please, insert a password")

    # Check password match
    elif password != confirm_password:
        return render_template("register.html", error=True, message="ERROR: Passwords don't match")

    # Check if username exists
    elif db.execute("SELECT * FROM users WHERE username = :username", username=username):
        return render_template("register.html", error=True, message="ERROR: This username has been already taken")

    # Secure password and insert it with the username
    hashed_password = generate_password_hash(password)
    db.execute("INSERT INTO users(username, hash) VALUES (:username, :hashed_password)",
        username=username, hashed_password=hashed_password)

    # Get the id
    rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)

    # Log user in session
    session["user_id"] = rows[0]["id"]

    # Go to home page
    return redirect("/")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user = session["user_id"]

    # If GET just view
    if request.method == "GET":
        # view transactions
        rows = db.execute("SELECT symbol, amount FROM stocks WHERE user_id = :user", user=user)

        # Create dictionary for stocks data owned
        stocks = {}
        for row in rows:
            stocks[row['symbol']] = row['amount']

        return render_template("sell.html", stocks=stocks)

    # I case of POST
    amount=int(request.form.get("amount"))
    symbol=request.form.get("symbol")
    price=lookup(symbol)["price"]
    value=round(price * float(amount))

    # Update stocks table
    stocks_before = db.execute("SELECT amount FROM stocks WHERE user_id = :user AND symbol = :symbol", symbol=symbol, user=user)[0]['amount']
    stocks_after = stocks_before - amount

    # not enough
    if stocks_after < 0:
        return render_template("sell.html", error=True, message="You can't sell more than you have")

    # delete stock
    elif stocks_after == 0:
        db.execute("DELETE FROM stocks WHERE user_id = :user AND symbol = :symbol", symbol=symbol, user=user)

    # or update it
    else:
        db.execute("UPDATE stocks SET amount = :amount WHERE user_id = :user AND symbol = :symbol", symbol=symbol, user=user, amount=stocks_after)

    # update cash and history
    cash = db.execute("SELECT cash FROM users WHERE id = :user", user=user)[0]['cash']
    cash_after = cash + price * float(amount)
    db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash=cash_after, user=user)
    db.execute("INSERT INTO transactions(user_id, symbol, amount, value) VALUES (:user, :symbol, :amount, :value)",
        user=user, symbol=symbol, amount=-amount, value=value)

    # If success redirect
    return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
