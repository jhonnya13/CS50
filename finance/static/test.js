CREATE TABLE projects (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    file_name TEXT NOT NULL,
    file_size NUMERIC NOT NULL,
    file_path TEXT NOT NULL,
    project_name TEXT NOT NULL,
    user_id NUMERIC NOT NULL,
    upload_date TEXT,
    FOREIGN KEY (user_id) REFERENCES users(id));
