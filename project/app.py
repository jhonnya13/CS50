import os
import zipfile
#import openpyxl

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, session, request, url_for, jsonify, send_from_directory
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from werkzeug.utils import secure_filename
from helpers import apology, login_required, generate_thumbnails, create_excel


# Configure application
app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///project.db")

# Set folder for videos
UPLOAD_FOLDER = 'files/video'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

app.config['MAX_CONTENT_LENGTH'] = 100 * 1024 * 1024


@app.after_request
def after_request(response):
    '''
    Ensure responses aren't cached

    '''

    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/login", methods=["GET", "POST"])
def login():
    '''
    Log user in

    '''

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
    '''
    Log user out

    '''

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")




@app.route("/")
@login_required
def index():
    '''
    Show new project

    '''
    return render_template("new_project.html")




@app.route("/register", methods=["GET", "POST"])
def register():
    '''
    Register user

    '''

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


@app.route("/settings", methods=["GET", "POST"])
def settings():
    '''
    User settings to change password

    '''

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



@app.route('/project/<project_name>', methods=["GET"])
def project(project_name):
    '''
    Show project information

    '''

    user_id = session["user_id"]
    project_details = {'name': project_name}
    file_details = db.execute('''
                              SELECT file_name
                              FROM projects
                              WHERE project_name = ? AND user_id = ?''', project_details['name'], user_id)

    return render_template('project.html', project=project_details, files=file_details)  # return render_template("project.html")



# Help function to redirect
@app.route('/submit_project', methods=["POST"])
def submit_project():
    '''
    Submit project

    '''

    project_name = request.form.get('project_name')
    if project_name:
        return redirect(url_for('project', project_name=project_name))
    return redirect(url_for('/'))  # or handle the error appropriately




@app.route("/upload", methods=["POST"])
def upload_file():
    '''
    Upload files

    '''

    if 'file' not in request.files:
        return jsonify({'error': 'No file'}), 400

    file = request.files['file']
    if file.filename == '':
        return jsonify({'error': 'No selected file'}), 400

    # Request project name and user
    project_name = request.form.get("project_name")
    user_id = str(session["user_id"])


    # Checking server site that files are .mov
    if file and file.filename.endswith('.mov'):
        # Create project directory
        project_folder = os.path.join(app.config['UPLOAD_FOLDER'], user_id, project_name)
        os.makedirs(project_folder, exist_ok=True)

        filename = secure_filename(file.filename) # Ensure that filename is safe to use
        file_path = os.path.join(project_folder, filename) # video/user_id/project_name/
        file.save(file_path)

        # Add file name, user and project to the database
        insert_sql = """INSERT INTO projects (file_name, file_size, file_path, project_name, user_id, upload_date)
               VALUES (?, ?, ?, ?, ?, ? );"""
        upload_date = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        file_size = request.form.get('file_size')
        db.execute(insert_sql, filename, file_size, file_path, project_name, user_id, upload_date)

        # Return success
        return jsonify({'success': 'File uploaded successfully'}), 200
    else:
        return jsonify({'error': 'File type not allowed'}), 400




@app.route("/projects")
@login_required
def projects():
    '''
    Show all projects

    '''

    user_id = session["user_id"]
    projects_list = db.execute('''
                          SELECT project_name, COUNT(*) AS file_count,
                               MAX(upload_date) AS upload_date
                          FROM projects
                          WHERE user_id = ?
                          GROUP BY project_name
                          ORDER BY upload_date DESC''', user_id)
    return render_template("projects.html", projects_list=projects_list)



@app.route("/generate-thumbnails")
@login_required
def generatethumbnails():
    # Java had project name in it
    project_name = request.args.get('project_name')
    user_id = session["user_id"]
    user_id = str(user_id)

    # video_dir = files/video/1/Test
    video_dir = os.path.join(app.config['UPLOAD_FOLDER'], user_id, project_name)
    thumbnail_dir = os.path.join(video_dir, "thumbnails")
    if not os.path.exists(thumbnail_dir):
        os.makedirs(thumbnail_dir)

    '''
        Itterate thro videos and create Thumbnail (resized)
        and save to zip
    '''

    # Provide both paths and value to resize
    paths = generate_thumbnails(video_dir,thumbnail_dir,resize=2)[0]


    # Zip files
    zip_name = "Thumbnails.zip"
    zip_path = os.path.join(thumbnail_dir, zip_name)

    with zipfile.ZipFile(zip_path,'w') as zipf:
    # Add files to zip file
        for name,path in paths.items():
            zipf.write(path, arcname=name)

    return send_from_directory(thumbnail_dir, zip_name)




@app.route("/generate-excel")
@login_required
def generateexcel():
    '''
    Create Excel file

    '''

    project_name = request.args.get('project_name')    # Java had project name in it
    user_id = session["user_id"]
    user_id = str(user_id)

    # video_dir = files/video/1/Test
    video_dir = os.path.join(app.config['UPLOAD_FOLDER'], user_id, project_name)
    thumbnail_dir = os.path.join(video_dir, "thumbnails")
    if not os.path.exists(thumbnail_dir):
        os.makedirs(thumbnail_dir)

    '''
        Itterate thro videos and create Thumbnail (resized)
        and save to Excel
    '''

    # Provide both paths and value to resize
    # Function will get back thumbnail_path
    result = generate_thumbnails(video_dir,thumbnail_dir,resize=10)
    duration = result[1]

    #thumbnail_dir = files/video/1/Test/thumbnails
    create_excel(thumbnail_dir, data=duration)

    # Serve the Excel file to the user
    return send_from_directory(thumbnail_dir, "Excel.xlsx")
