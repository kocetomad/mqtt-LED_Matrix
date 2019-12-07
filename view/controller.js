
    var grid=[];
    var gridExport="";
    function Box(x,y){
      this.x=x;
      this.y=y;
      this.width=20;
      this.height=20;
      this.fill='#FFFFFF';
      this.display = function(){
        stroke(1);
        fill(this.fill);
        rect(this.x,this.y,this.width,this.height);
      }
      this.clicked= function(){
        if(mouseX>=this.x && mouseX<=this.x+this.width 
        && mouseY>=this.y && mouseY<=this.y+this.height
        && mouseIsPressed){
          this.fill='#FF0000';
          gridExport=gridExport+""+this.x/20+","
        console.log(gridExport);
        }
        
      }

    }

    function setup(){
    for(var i=0;i<8;i++){
      for(var z=0;z<32;z++){
        grid.push(new Box(20*z,20*i))
      }
    }
    let cnv=createCanvas(1000, 1000);
    cnv.position(10, 100);
    var button = select('#setter');
    button.mousePressed(setText);
    //Socket connection
    console.log("asdasd");

    this.socket=io.connect(document.location.host);//document.location.href
    let placeholder='a';
    let data={
        txt:placeholder
      };
      this.socket.emit('dataIn',data);
    }

    function draw(){
      for (var i = 0; i < grid.length; i++) {
        grid[i].display(); 
          
        if(grid[i].clicked()){
          console.log("gridExport.toString");

        }    
      }
    }

    function setText(){
        
          //end
      
          //Wake and update the socket event
          var textBox=select('#myText').value();
          console.log(gridExport.toString);
          let data={
            txt:gridExport+""
          };
          httpPost('setMatrix/',data,'json');
    }

    

   