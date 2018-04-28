var canvas = document.getElementById('result');
canvas.width = innerWidth;
canvas.height = innerHeight;
var ctx = canvas.getContext('2d');
var points = [];
var SCALE = 10;
var polygonLength = -1; // Switching b/w polygon construction and marking of start and end;


function mark(point) {
    ctx.beginPath();
    ctx.arc(innerWidth / 2 + point[0] * SCALE, innerHeight / 2 - point[1] * SCALE, 2, 0, 2 * Math.PI);
    ctx.closePath();
    ctx.fill();
}

function moveTo(point) {
    ctx.moveTo(innerWidth / 2 + point[0] * SCALE, innerHeight / 2 - point[1] * SCALE);
}

function lineTo(point) {
  console.log(point);
    ctx.lineTo(innerWidth / 2 + point[0] * SCALE, innerHeight / 2 - point[1] * SCALE);
}

function colorString(color) {
    return "rgba(" +
        ~~(255 * color[0]) + "," +
        ~~(255 * color[1]) + "," +
        ~~(255 * color[2]) +
        ",1)";
}
canvas.onclick = function(e) {
    mark([(e.pageX - innerWidth / 2) / SCALE, (innerHeight / 2 - e.pageY) / SCALE]);
    points.push([(e.pageX - innerWidth / 2) / SCALE, (innerHeight / 2 - e.pageY) / SCALE]);
    if (points.length > 1 && polygonLength == -1) {
      ctx.beginPath();
      moveTo(points[points.length - 2]);
      lineTo(points[points.length - 1]);
      ctx.closePath();
      ctx.stroke();
    }
    if(points.length == polygonLength + 2 && polygonLength != -1) {
      getInput();
    }
}
canvas.oncontextmenu = function() {
  ctx.beginPath();
  moveTo(points[points.length - 1]);
  lineTo(points[0]);
  ctx.closePath();
  ctx.stroke();
  polygonLength = points.length;
  return false;
}

function getInput() {
    var res = "";
    res += polygonLength;
    points.forEach(function(item) {
        res += "\n" + item[0] + " " + item[1];
    });
    document.getElementById('input').value = res;
    document.getElementById('exchange').style.display = "block";
    document.getElementById('input').select();
    document.execCommand('Copy');
}
function DrawPath() {
  var output = document.getElementById('output').value.trim().split("\n");
  document.getElementById('exchange').style.display = "none";
  ctx.beginPath();
  moveTo(points[polygonLength]);
  for(i = 0; i < output.length-1; i++) {
    lineTo(points[output[i]]);
  }
  lineTo(points[polygonLength + 1]);
  ctx.strokeStyle = colorString([0.5, 0.5, 1.0]);
  ctx.strokeWidth = 2;
  ctx.stroke();
  ctx.closePath();
  document.getElementById('dist').innerHTML = output[output.length - 1];
}