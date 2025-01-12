import QtQuick
import QtLocation
import QtPositioning

Rectangle {
    id: mapWindow
    width: 800
    height: 600

    property double _latitude:51.5072
    property double _longitude:0.1276

    ListModel {
        id: markersModel
    }

    Plugin {
        id: openmapview
        name: "osm"
    }

    Map {
        id: mapview
        anchors.fill: parent
        plugin: openmapview
        center: QtPositioning.coordinate(mapWindow._latitude, mapWindow._longitude)
        zoomLevel: 5

        WheelHandler
        {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland" ? PointerDevice.Mouse | PointerDevice.TouchPad : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }

        DragHandler
        {
            id: drag
            target: null
            onTranslationChanged: (delta) => mapview.pan(-delta.x, -delta.y)
        }

        MapItemView {
            model: markersModel
            delegate: MapQuickItem {
                id: marker
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
                width: 50
                height: 50
                sourceItem: Item {
                    width: parent.width
                    height: parent.height

                    // Background Circle
                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "transparent"
                        border.color: "black"
                        border.width: 2
                        radius: parent.width / 2
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#ff8c00" }  // Orange to yellow gradient
                            GradientStop { position: 1.0; color: "#ffcc00" }
                        }
                    }

                    Rectangle {
                        width: parent.width * 0.8
                        height: 30
                        color: "black"
                        radius: 5
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: -10

                        Text {
                            anchors.centerIn: parent
                            text: {
                                    if (model.hostname && model.hostname !== "") {
                                        return model.ip + "\n" + model.hostname;
                                    } else {
                                        return model.ip;
                                    }
                                }
                            color: "black"
                            font.pixelSize: 12
                            font.bold: true
                            wrapMode: Text.Wrap
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }

    }
    function addLocationMarking(lat, lon, param_ip, param_hostname)
    {
        markersModel.append({ latitude: lat, longitude: lon, ip: param_ip, hostname: param_hostname });
    }

    function changeMapPosition(lat, lon)
    {
        mapWindow._latitude = lat;
        mapWindow._longitude = lon;
        mapview.center = QtPositioning.coordinate(lat, lon);
    }

    function removeLocationMarking(lat, lon, ip)
    {
        var indexToRemove = -1;
        for (var i = 0; i < markersModel.count; i++)
        {
            var markerData = markersModel.get(i);
            if (markerData.latitude === lat && markerData.longitude === lon && markerData.ip === ip) {
                indexToRemove = i;
                break;
            }
        }

        if (indexToRemove !== -1) {
            markersModel.remove(indexToRemove);
        }
    }
}
