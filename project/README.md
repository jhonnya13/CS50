# BID SHEET
#### Video Demo:  https://youtu.be/86LmrHJJBCk
#### Description:
A web platform designed for the VFX industry to generate thumbnails and Excel sheets. Users can upload .mov files and automatically generate single images from them. The images will be resized (you can change the resize value in the generate_thumbnails function in helpers.py). Additionally, an Excel file can be generated with multiple columns like ["№", "SHOT PREVIEW", "SHOT NAME", "DURATION", "VFX SCOPE OF WORK DESCRIPTION", "DAYS", "COST"]. These columns can be adjusted in the create_excel function in helpers.py. Uploaded files are stored in the "file/video/user_id" folder, and information about these files is uploaded into the database.
When user want to create New Project, program will ask for a name and files. It will only accept .mov files and ignore others. If user press Upload without providing Project Name,
the programm will block this and request user to provide one. Once Upload pressed, user can not change Project Name. After files start uploading the progress bar will apeear. Once file uploaded
file's name turns green. When all files uploaded Generate button will appear.


> [!NOTE]
> Users password can't be less than 8 characters.


## Code:
#### Database:
The database, project.db, has two tables:

- Users Table: Contains user name, id, and hash.
- Projects Table: Contains id, filename, upload date, uploader's user id, and file size.


#### Python:
helpers.py
contains multiple function to to streamline the workflow:
- apology(message, code=400):
    Render message as an apology to user instead of scary error message. In the code only used if user types wrong password,
    or if user with that name and passport already exists.

- login_required(f):
    Decorate routes to require login.

- generate_thumbnails(video_dir,thumbnail_dir,resize=2):
    Provide paths and value to resize. Generate thumbnails for videos in a choosen directory.

- create_excel
    Create an Excel file with image data and multiple columns

app.py
- route("/projects")
    lists all the projects that user has. User can choose one and be redirected to that project page

- route('/project/<project_name>'
    unique route to see files uploaded for the exact project. Here user can see files and generate thumbnail or Excel.


- route("/generate-thumbnails")
    If user choose to generate thumbnails this route called using AJAX. It generate pictures and than stores them in .zip. Once done it sends request back
    and Java Script initiate download

- route("/generate-excel")
    If user choose to generate Excel. Foolow to the page where excel will be generated. It uses AJAX to receive call from button,
    generate thumbnails and that create and Excel. Than it sends request back and Java Script initiate download

- route("/settings"
    Handles scenario where user want to change password

#### Java Script:
Multiple Java files to handle drag and drop, upload and download.

- fileHandler.js - Manages files in the drag-and-drop area.
                    Highlights the area when hovered with files, checks for valid extensions (.mov), and displays the filenames to the user.
- upload.js - Uploads files to the server.
- progressBar.js - Shows the upload progress bar when Upload pressed.
- index.js - A collection of all the JavaScript files above.
- generate.js - contains two event listeners responsible to call for Thumbnails or Excel generation in app.py.


#### CSS:
For design I heavily relied on bootstrap_morph theme and overrided that only for a few details where morph wasn't working or didn't have a style at all
like drop-area and file-list for example.
For more visiability once file is uploaded it's font turns green with .uploaded style



Contact
Kseniia - jhonnya13@gmail.com
