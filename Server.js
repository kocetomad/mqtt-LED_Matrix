var express = require('express');
var bodyParser = require('body-parser')
var app = express();

var PORT=80;

var server = app.listen(PORT, listening);

// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: false }))

// parse application/json
app.use(bodyParser.json())


//MQTT
var mqtt = require('mqtt');
var client  = mqtt.connect('mqtt://127.0.0.1');
client.on('connect', function () {
  
client.publish('mytopic/test', 'Hello mqtt');
console.log('Message Sent');
});

//REST HANDLING

app.post('/setMatrix',setMatrix);

function setMatrix(request,response){
  console.log(request.body.txt);
  client.publish('mytopic/test', request.body.txt);

}



app.use('/', express.static("view"));

function listening() {
  console.log("Http on on port:"+PORT);
}