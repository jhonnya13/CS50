// This code written with the help of ChatGTP

import { handleFiles, handleDrop, preventDefaults, highlight, unhighlight } from './fileHandler.js';
import { uploadFiles } from './upload.js';



// Once all uploaded, establish elements
document.addEventListener('DOMContentLoaded', function() {
    const form = document.getElementById('file-form');
    const dropArea = document.getElementById('drop-area');
    const fileList = document.getElementById('file-list');
    const uploadBtn = document.getElementById('upload-btn');
    window.filesToUpload = []; // Make this variable global


    // Listens for SUBMIT event (Generate button)
    form.addEventListener('submit', function(event) {
        event.preventDefault(); // Prevent form submission (stop default behavior, allowing to handle the form submission with JavaScript instead)

        const projectName = document.querySelector('input[name="project_name"]').value.trim(); // trim - to remove white spaces before and after

        if (!projectName) {
            alert('Project name is required.');
            return;
        }

        if (fileList.children.length === 0) {
            alert('At least one file must be uploaded.');
            return;
        }

        // After file uploads, proceed with form submission
        form.submit(); // This will submit the form to the specified action URL


    });


    ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
        dropArea.addEventListener(eventName, preventDefaults, false);
    });

    ['dragenter', 'dragover'].forEach(eventName => {
        dropArea.addEventListener(eventName, highlight, false);
    });

    ['dragleave', 'drop'].forEach(eventName => {
        dropArea.addEventListener(eventName, unhighlight, false);
    });

    dropArea.addEventListener('drop', handleDrop, false);

    // Add event listener for the upload button
    uploadBtn.addEventListener('click', uploadFiles);




});
