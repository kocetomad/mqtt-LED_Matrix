var express = require('express');
var bodyParser = require('body-parser')
var app = express();
var MATRIX_TEXT="empty";
var PORT=80;

var server = app.listen(PORT, listening);

// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: false }))

// parse application/json
app.use(bodyParser.json())


//MQTT
var mqtt = require('mqtt');
var options = {
  port: 1883,
    host: '51.105.190.58',
    username: 'sammy',
    password: 'pass',
   
}
var client  = mqtt.connect('mqtt://51.105.190.58',options);
client.on('connect', function () {

client.publish('mytopic/test',MATRIX_TEXT);
});


client.on('mytopic/test', function () {

client.publish('mytopic/test', MATRIX_TEXT);
});


//REST HANDLING

app.get('/refresh',refresh);
function refresh(request,response){
  client.publish('mytopic/test',MATRIX_TEXT);
   console.log(MATRIX_TEXT);
  response.send(MATRIX_TEXT);	
}

app.post('/setMatrix',setMatrix);

function setMatrix(request,response){
  
  MATRIX_TEXT=request.body.txt;	
  console.log(request.body.txt);
  client.publish('mytopic/test',MATRIX_TEXT);

}


app.use('/', express.static("view"));

function listening() {
  console.log("Http on on port:"+PORT);
}
