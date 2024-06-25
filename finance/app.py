import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash


from helpers import apology, login_required, lookup, usd
from datetime import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks for user"""

    user_id = session["user_id"]
    user_own_list = []
    owned = db.execute('''
                       SELECT symbol, SUM(shares)
                       AS total_shares
                       FROM history
                       WHERE user_id = ?
                       GROUP BY symbol
                       HAVING total_shares != 0''', user_id)

    total_price = 0
    for row in owned:
        symbol = row['symbol']
        symbol_info = lookup(symbol)
        # symbol_info = {symbol :XOM, price: 12}
        symbol_info['total_shares'] = row['total_shares']
        # symbol_info = {symbol :XOM, price: 12, total_shares:3}
        # и теперь этот дикт мы должны добавить в лист
        symbol_info['total_price'] = row['total_shares']*symbol_info['price']
        user_own_list.append(symbol_info)
        total_price += symbol_info['total_price']

    user_cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
    grand_total = user_cash + total_price
    return render_template("index.html", result_list=user_own_list, user_cash=user_cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        # Show the form
        return render_template("buy.html")

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check for valid input:
        if not symbol:
            return apology("must provide stock name")

        if not shares:
            return apology("must specify amount of shares")

        # Check shares more than 0
        try:
            shares = int(shares)
            if (shares <= 0):
                return apology("shares must be positive integer")
        except ValueError:
            return apology("shares must be positive integer")


        # Get back price and symbol dictionary from lookup

        symbol_price = lookup(symbol)

        # If empty show apology
        if symbol_price == None:
            return apology("No stock found")

        '''
            We have shares, symbol and stock_price,
            Let's check if user has enough money

        '''
        # Ckeck users cash
        user_id = session["user_id"]
        user_cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
        total = symbol_price["price"] * shares

        # Check if he/she can afford share
        if (user_cash < total):
            return apology("Not enough money")

        # Add purchuase to a database
        else:
            # SQL insert statement
            insert_sql = '''
            INSERT INTO history (user_id, symbol, shares, price, total, created_at)
            VALUES (?, ?, ?, ?, ?, ?);
            '''

            current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            db.execute(insert_sql, user_id,
                       symbol_price["symbol"], shares, symbol_price["price"], total, current_time)

            # Update cash in users table
            user_cash = user_cash - total
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash, user_id)

            # All DONE!
            # Redirect user to home page
            flash('Bought!')
            # или редирект?
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    history = db.execute('''
                       SELECT *
                       FROM history
                       WHERE user_id = ?
                       ''', user_id)

    print(history)
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/settings", methods=["GET", "POST"])
def settings():
    """User settings to change password"""
    user_id = session["user_id"]

    if request.method == "GET":
        return render_template("settings.html")

    if request.method == "POST":
        password = request.form.get("password")
        if not password:
            return apology("Missing passwod")

        new_password = request.form.get("new_password")
        if not new_password:
            return apology("Missing new password")

        confirmation = request.form.get("confirmation")
        if new_password != confirmation:
            return apology("Confirmation doesn't match")

        # Check if old password is valid
        hashed_password = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]["hash"]
        if check_password_hash(hashed_password, password):

            # Hash new password
            hashed_password = generate_password_hash(new_password)

            # Update database with hashed NEW password
            db.execute("UPDATE users SET hash = ? WHERE id = ?", hashed_password, user_id)
            flash('Password Updated')
            return redirect("/")

        else:
            return apology("Not correct password")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":
        symbol = request.form.get("symbol")

        # Get back price and symbol dictionary from lookup
        symbol_price = lookup(symbol)
        # If empty show apology
        if symbol_price == None:
            return apology("No stock found")
        # If not empty pass this to the page
        else:
            return render_template("quoted.html", symbol_price=symbol_price)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("register.html")

    # User reached route via POST (as by submitting a form via POST)
    elif request.method == "POST":
        # Check username
        username = request.form.get("username")
        if not username:
            return apology("Provide user name")

        # Check if username already exists
        if db.execute("SELECT * FROM users WHERE username = ?", username):
            return apology("Username already exists")

        # Check password
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password:
            return apology ("Provide password")
        if password != confirmation:
            return apology ("Confirmation doesn't match")

        # If password and name correct
        else:
            hashed_password = generate_password_hash(password)
            # Check if user already exists by inserting name into database
            try:
                user_id = db.execute(
                    "INSERT INTO users (username,hash) VALUES (?,?)", username, hashed_password)
                session["user_id"] = user_id
                flash('Register')
                return redirect("/")

            except ValueError:
                return apology("Error")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    stocks = db.execute(
        "SELECT symbol, SUM(shares) AS total_shares FROM history WHERE user_id = ? GROUP BY symbol", user_id)

    if request.method == "GET":
        return render_template("sell.html", result_list=stocks)

    if request.method == "POST":
        # Check user input
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Missing stock")

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing shares")
        shares = int(shares)
        if (shares < 0):
            return apology("Can't be negative")

        # See how many stocks user has
        stock = db.execute(
            "SELECT symbol, SUM(shares) AS total_shares FROM history WHERE user_id = ? AND symbol =? GROUP BY symbol", user_id, symbol)
        if shares > stock[0]['total_shares']:
            return apology("Can't sell more than you have")
        else:
            '''
                We checked that user can sell that amount of stocks,
                Let's sell
            '''

            # Check current price
            symbol_price = lookup(symbol)
            total = symbol_price['price'] * shares

            # Sell stock
            # SQL insert statement
            insert_sql = '''
            INSERT INTO history (user_id, symbol, shares, price, total, created_at)
            VALUES (?, ?, ?, ?, ?, ?);
            '''

            current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            db.execute(insert_sql, user_id,
                       symbol_price["symbol"], -shares, symbol_price["price"], total, current_time)

            user_cash = float(db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"])
            # Update cash in users table
            user_cash = user_cash + total
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash, user_id)

            # All DONE!
            # Redirect user to home page
            flash('Sold!')
            # или редирект?
            return redirect("/")
