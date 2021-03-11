import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import Li 1.0 as Li
ApplicationWindow {
    id:root
    visible: true
    Li.Interaction { id : interaction }
    property int fieldWidth:1200;
    property int fieldHeight:900;
    width: fieldWidth;
    height: fieldHeight;
    minimumHeight: fieldHeight;
    minimumWidth: fieldWidth;
    title: qsTr("vision_plugin")
    Item{
        id:left;
        height:parent.height;
        width:parent.width;
        Li.Field{
            id:field;
            width:parent.width;
            height:parent.height;
            function resize(){ resetsize(width,height); }
            onWidthChanged: {resize();}
            onHeightChanged: {resize();}
        }
    }
    Timer{
        id:fpsTimer;
        interval:13;
        running:true;
        repeat:true;
        onTriggered: {
            field.repaint();
            //interaction.sendCommand();
        }
    }
}
