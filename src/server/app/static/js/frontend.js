function get_time() {
    var today = new Date();
    var h = today.getHours();
    var m = today.getMinutes();
    if (m < 10) {
        m = "0" + m;  // add zero in front of numbers < 10
    }

    $('#time_text').text(h + ":" + m);
    var t = setTimeout(get_time, 1000);
}

function get_msg(lineno) {
    $.ajax({
        url: "http://127.0.0.1:5000/api/line/" + lineno,
        success: function (data) {
            var id = 'msg' + lineno + '_text';
            $('#' + id).text(data.text);
            id = 'msg' + lineno + '_icon';
            if (data.icon_url === "") {
                $('#' + id).hide();
            }
            else {
                $('#' + id).show();
                $('#' + id).attr("src", data.icon_url);
            }
            var seconds = data.seconds * 1000;
            if (seconds === 0) {
                seconds = 60000;
            }
            if (lineno === 0) {
                var t = setTimeout(get_top_msg, seconds);
            }
            else if (lineno === 1) {
                var t = setTimeout(get_bottom_msg, seconds);
            }
        }
    });
}

function get_top_msg() {
    get_msg(0);
}

function get_bottom_msg() {
    get_msg(1);
}


$(document).ready(function () {
    get_top_msg();
    get_bottom_msg();
    get_time()
});