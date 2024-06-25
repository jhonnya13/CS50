// This code written with the help of ChatGTP

document.getElementById('genTBtn').addEventListener('click', function() {
    var xhr = new XMLHttpRequest();
    var project_name = this.getAttribute('data-project-name');
    console.log(project_name);
    xhr.open('GET', '/generate-thumbnails?project_name=' + encodeURIComponent(project_name), true);
    xhr.onload = function() {
        if (this.status == 200) {

            let filename = 'Thumbnail.zip';
            let url = '/generate-thumbnails?project_name=' + encodeURIComponent(project_name);

            var a = document.createElement('a');
            a.href = url;
            a.download = 'Thumbnails.zip';
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
        }
    };
    xhr.send();

});

document.getElementById('genEBtn').addEventListener('click', function() {
    var xhr = new XMLHttpRequest();
    var project_name = this.getAttribute('data-project-name');
    console.log(project_name);
    xhr.open('GET', '/generate-excel?project_name=' + encodeURIComponent(project_name), true);
    xhr.onload = function() {
        if (this.status == 200) {

            let filename = 'Excel.xlsx';
            let url = '/generate-excel?project_name=' + encodeURIComponent(project_name);

            var a = document.createElement('a');
            a.href = url;
            a.download = 'Excel.xlsx';
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
        }
    };
    xhr.send();

});
