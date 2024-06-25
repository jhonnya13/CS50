// This code written with the help of ChatGTP

export function updateProgressBar(progress) {
    const progressBar = document.getElementById('progress-bar');
    progressBar.value = progress;
}

export function showProgressContainer() {
    const progressContainer = document.getElementById('progress-container');
    progressContainer.style.display = 'block';
}

export function hideProgressContainer() {
    const progressContainer = document.getElementById('progress-container');
    progressContainer.style.display = 'none';
}
