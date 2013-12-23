import QtQuick 1.1;
import "colors.js" as Colors;

Canvas {
    id: chart;
    anchors.left: reportBox.right;
    anchors.top: reportBox.top;
    anchors.leftMargin: 50;
    width: App.screen_w * 0.35;
    height: reportBox.height * 0.5;
    property int sx : 0;
    property real sy : 0;

    property int xmax : 1;
    property real ymax : 1;

    property int ygrid_size : 10;
    property int xgrid_size : 10;


    antialiasing: true;
    property variant points : [];
    property variant labels : [];
    function flipY( _y ) { return (height - _y - 1); }

    onPointsChanged : requestPaint();

    function plot() {
        var peak = 0;
        for( var i in points ) {
            if( peak < points[i] )
                peak = points[i];
        }

        ymax = 1;
        while( peak/(ymax/10) > 10) { ymax *= 10 }
        //ymax = 1000;
        if( ymax - peak < 50 ) ymax += 100;
        sy = height / ymax;

        sx = width / (points.length + 1);

        var ctx = getContext('2d');

        ctx.save();
        ctx.clearRect(0, 0, width, height );
        ctx.lineWIdth = 0.5;
        drawGrid(ctx);

        ctx.restore();

        ctx.beginPath();
        ctx.lineWidth = 2;
        ctx.strokeStyle = "black";

        ctx.moveTo(1, flipY(1));
        var x = 0;
        for( i = points.length; i >= 0; i-- ) {
            ctx.lineTo(x * sx, flipY(chart.points[i] * sy) );
            x++;
        }

        ctx.stroke();
        ctx.beginPath();
        x = 0;
        //Visual points & labels
        ctx.lineWidth = 1.5;
        ctx.font = "15px sans-serif";
        for( i = labels.length; i >= 0; i-- ) {
            //label
            ctx.text( chart.labels[i], x * sx - 10, flipY(chart.points[i] * sy) - 10);
            x++;
        }
        ctx.stroke();
        ctx.strokeText();
        ctx.beginPath();
        ctx.fillStyle = Colors.main_blue;
        x = 0;
        for( i = points.length; i >= 0; i-- ) {
            //point
            ctx.ellipse(x * sx - 10, flipY(chart.points[i] * sy)- 10, 20, 20);
            x++;
        }
        ctx.fill();
        ctx.restore();
    }

    function drawGrid(ctx) {
        ctx.beginPath();
        ctx.strokeStyle = "white";
        ctx.moveTo( 1   , 1 );
        ctx.lineTo( 1   , height - 1 );
        ctx.lineTo( width - 1, height -  1 );
        ctx.lineTo( width - 1,1 );
        ctx.lineTo( 1, 1 );

        for( var y = 0; y < ygrid_size; y++ ) {
            ctx.moveTo( 1, y * (height / ygrid_size) );
            ctx.lineTo( width, y * (height / ygrid_size));
        }

        for( var x = 0; x < (points.length + 1); x++ ) {
            ctx.moveTo( x * (width / (points.length + 1)), 1 );
            ctx.lineTo( x * (width / (points.length + 1)), width);
        }

        ctx.stroke();
    }
}
