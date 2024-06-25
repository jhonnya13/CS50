// This code written with the help of ChatGTP

import { updateProgressBar, showProgressContainer, hideProgressContainer } from './progressBar.js';

export function addFileToList(file) {
    const fileList = document.getElementById('file-list');
    let fileDiv = document.createElement('div');
    fileDiv.textContent = file.name;
    fileDiv.id = `file-${file.name}`; // Add a unique ID based on the file name
    fileList.appendChild(fileDiv);
}

export async function uploadFiles() {
    const projectName = document.querySelector('input[name="project_name"]').value.trim(); // trim - to remove white spaces before and after
    if (!projectName) {
        alert('Project name is required.');
        return;
    }

    // Not allow to change project name after uploading
    document.querySelector('input[name="project_name"]').setAttribute('readonly', true);

    const filesToUpload = window.filesToUpload;
    showProgressContainer();


    for (const file of filesToUpload) {
        let formData = new FormData();
        formData.append('file', file);
        formData.append('project_name', projectName);
        formData.append('file_size', file.size);

        try {
            let response = await fetch('/upload', {
                method: 'POST',
                body: formData
            });

            const reader = response.body.getReader();
            const contentLength = +response.headers.get('Content-Length');
            let receivedLength = 0;
            let chunks = [];

            while (true) {
                const { done, value } = await reader.read();
                if (done) break;
                chunks.push(value);
                receivedLength += value.length;

                // Calculate progress
                const progress = (receivedLength / contentLength) * 100;
                updateProgressBar(progress);
            }

            const blob = new Blob(chunks);
            const text = await blob.text();
            const data = JSON.parse(text);

            console.log('File upload response:', data);
            if (data.success) {
                console.log('File uploaded successfully.');
                document.getElementById(`file-${file.name}`).classList.add('uploaded'); // Add the uploaded class (turns to green)
            } else {
                console.error('File upload failed:', data.error);
            }
        } catch (error) {
            console.error('Error uploading file:', error);
        }
    }
    hideProgressContainer();

    // Show the submit button
    submitBtn.style.display = 'inline';
}
