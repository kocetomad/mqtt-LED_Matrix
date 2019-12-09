
    var grid=[];
    var gridExport="";
    function Box(x,y){
      this.x=x;
      this.y=y;
      this.width=20;
      this.height=20;
      this.fill='#FFFFFF';
      this.isClicked=false;
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
          
            gridExport=gridExport+""+this.x/20+","+this.y/20+",";
            this.isClicked=true;

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
    let cnv=createCanvas(1000, 161);
    cnv.position(10, 100);
    var button = select('#setter');
    button.mousePressed(setText);
    var button2 = select('#picSetter');
    button2.mousePressed(setPic);
  
    }

    function draw(){
      for (var i = 0; i < grid.length; i++) {
        grid[i].display(); 
          
        if(!grid[i].isClicked){
          grid[i].clicked();
        }    
      }
    }

    function setText(){
         var textBox=select('#myText').value();
          let data={
            txt:textBox
          };
          httpPost('setMatrix/',data,'json');
    }

    function setPic(){
     gridExport=gridExport.substring(0, gridExportx.length - 1);
      let data={
        txt:gridExport
      };
      httpPost('setMatrix/',data,'json');
    }

    

   