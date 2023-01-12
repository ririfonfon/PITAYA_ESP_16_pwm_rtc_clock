window.addEventListener('load', setup);

var connection;

function connect() {
    console.log('connect()');
    connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);

    connection.onopen = function() {
        connection.send('Connect ' + new Date());
    };

    connection.onmessage = function(e) {
        console.log('Server: ', e.data);

        if (e.data.charAt(0) == 'cc') {
            var data = e.data.split(':');
            document.getElementById(data[0]).value = data[1];
        } //if

        if (e.data.charAt(0) == 'mem') {
            var val = e.data.split(':')[1];
            var elems = document.querySelectorAll('#mode li a');
            [].forEach.call(elems, function(el) {
                el.classList.remove('active');
            });
            document.getElementById(val).classList.add('active');
        } //if

        if (e.data.charAt(0) == 'b') {

            if (e.data.charAt(1) == 'a') {
                var data = e.data.split(':');
                document.getElementById('cc1').value = data[1];
            } //if a

            if (e.data.charAt(1) == 'b') {
                var data = e.data.split(':');
                document.getElementById('cc2').value = data[1];
            } //if b

            if (e.data.charAt(1) == 'c') {
                var data = e.data.split(':');
                document.getElementById('cc3').value = data[1];
            } //if c

            if (e.data.charAt(1) == 'd') {
                var data = e.data.split(':');
                document.getElementById('cc4').value = data[1];
            } //if d

            if (e.data.charAt(1) == 'e') {
                var data = e.data.split(':');
                document.getElementById('cc5').value = data[1];
            } //if e

            if (e.data.charAt(1) == 'f') {
                var data = e.data.split(':');
                document.getElementById('cc6').value = data[1];
            } //if e



        } //if b

    }; //function (e)

    connection.onclose = function(e) {
        console.log('Socket is closed. Reconnect will be attempted in 1 second.', e.reason);
        connection.close();
        setTimeout(function() {
            connect();
        }, 1000);
    };

    connection.onerror = function(err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        connection.close();
        setTimeout(function() {
            connect();
        }, 1000);
    };
}

connect();

id_array = new Array('cc1');
values = new Array(id_array.length);


////////////////////////////////////////////////////////////////prepare

function prepareVar1() {
    var a = parseInt(document.getElementById('cc1').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "ba" + values;
    console.log('eData: ' + data);
    connection.send(data);
} //prepare 1

function prepareVar2() {
    var a = parseInt(document.getElementById('cc2').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bb" + values;
    console.log('fData: ' + data);
    connection.send(data);
} //prepare 2

function prepareVar3() {
    var a = parseInt(document.getElementById('cc3').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bc" + values;
    console.log('gData: ' + data);
    connection.send(data);
} //prepare 3

function prepareVar4() {
    var a = parseInt(document.getElementById('cc4').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bd" + values;
    console.log('hData: ' + data);
    connection.send(data);
} //prepare 4

function prepareVar5() {
    var a = parseInt(document.getElementById('cc5').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "be" + values;
    console.log('iData: ' + data);
    connection.send(data);
} //prepare 5

function prepareVar6() {
    var a = parseInt(document.getElementById('cc6').value).toString(8);
    if (a.length < 2) { a = '0' + a; }
    values = a;
    var data = "bf" + values;
    console.log('iData: ' + data);
    connection.send(data);
} //prepare 6


function submitVal(name, val) {
    var xhttp = new XMLHttpRequest();
    xhttp.open('GET', 'set?' + name + '=' + val, true);
    xhttp.send();
} //submitVal

function handle_M_B_S(e) {
    e.preventDefault();
    var name = e.target.className;
    var val = e.target.id;
    console.log('name: ' + name);
    console.log('val: ' + val);

    if (e.target.className.indexOf('mem') > -1) {
        elems = document.querySelectorAll('#mode li a');
        [].forEach.call(elems, function(el) {
            el.classList.remove('active');
            name = e.target.className;
        });
        e.target.classList.add('active');
    }
    submitVal(name, val);
} //handle_M_B_S

function rgbToHsl(r, g, b) {
    console.log('rgbToHsl(r, g, b)');
    r = r / 255;
    g = g / 255;
    b = b / 255;
    var max = Math.max(r, g, b);
    var min = Math.min(r, g, b);
    var h, s, l = (max + min) / 2;
    if (max == min) {
        h = s = 0;
    } else {
        var d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        switch (max) {
            case r:
                h = (g - b) / d + (g < b ? 6 : 0);
                break;
            case g:
                h = (b - r) / d + 2;
                break;
            case b:
                h = (r - g) / d + 4;
                break;
        }
        h = h / 6;
    }
    return [h, s, l];
}

function setup() {
    console.log('setup()');
    var xhttp = new XMLHttpRequest();

    elems = document.querySelectorAll('ul li a'); // adds listener
    [].forEach.call(elems, function(el) {
        // el.addEventListener('touchstart', handle_M_B_S, false);
        el.addEventListener('click', handle_M_B_S, false);
    });
    submitVal("mem", 0);
}

function RGBToHex(r, g, b) {
    if (r.length == 0) r = "00"
    if (g.length == 0) g = "00"
    if (b.length == 0) b = "00"

    r = parseInt(r).toString(16);
    g = parseInt(g).toString(16);
    b = parseInt(b).toString(16);

    if (r.length == 1) r = "0" + r;
    if (g.length == 1) g = "0" + g;
    if (b.length == 1) b = "0" + b;

    return "#" + r + g + b;
}

function hexToRGB(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}