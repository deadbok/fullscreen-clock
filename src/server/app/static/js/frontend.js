function startTime() {
    var today = new Date();
    var h = today.getHours();
    var m = today.getMinutes();
    m = checkTime(m);
    document.getElementById('time_display').innerHTML =
        h + ":" + m;
    var t = setTimeout(startTime, 1000);
}

function checkTime(i) {
    if (i < 10) {
        i = "0" + i;  // add zero in front of numbers < 10
    }
    return i;
}

function get_msg(lineno) {
    $.ajax({
        url: "http://127.0.0.1:5000/api/line/" + lineno //Get from Flask
    }).then(function (data) {
        $('#msg' + lineno + "_dispay").append(data.text);
        var seconds = $data.seconds;
    });
}

$(document).ready(function () {
    get_msg(0);
    get_msg(1);
});