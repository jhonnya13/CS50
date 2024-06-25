// This code written with the help of ChatGTP

import { addFileToList } from './upload.js';

export function preventDefaults(e) {
    e.preventDefault();
    e.stopPropagation();
}

export function highlight() {
    const dropArea = document.getElementById('drop-area');
    dropArea.classList.add('highlight');
}

export function unhighlight() {
    const dropArea = document.getElementById('drop-area');
    dropArea.classList.remove('highlight');
}

export function handleDrop(e) {
    let dt = e.dataTransfer;
    let files = dt.files;

    handleFiles(files);
    unhighlight(); // Remove highlight when files are dropped
}

export function handleFiles(files) {
    const fileList = document.getElementById('file-list');
    const uploadBtn = document.getElementById('upload-btn');
    files = [...files];
    window.filesToUpload = []; // Reset the list of files to upload

    files.forEach(file => {
        if (file.type === 'video/quicktime') { // Check if the file is a .mov file
            addFileToList(file); // Add to the list to show on the web
            window.filesToUpload.push(file); // Add to the list of files to upload
        } else {
            alert('Only .mov files are allowed.');
        }
    });

    if (fileList.children.length > 0) {
        fileList.style.display = 'block';
        uploadBtn.style.display = 'block'; // Show the upload button
    } else {
        fileList.style.display = 'none';
        uploadBtn.style.display = 'none'; // Hide the upload button
    }
}
