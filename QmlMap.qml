import QtQuick
import QtLocation
import QtPositioning
Rectangle
{
    id: window
    property double latitude: 53.20745
    property double longitude: 50.22456

    property Component locationMarker: locmarker

    Plugin {
            id: mapPlugin
            name: "osm"
            PluginParameter
            {
                name: "osm.mapping.providersrepository.disabled"
                value: "true"
            }
            PluginParameter {
                name: "osm.mapping.providersrepository.address"
                value: "http://maps-redirect.qt.io/osm/5.6/"
            }
        }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(latitude, longitude) // Oslo
        zoomLevel: 18
        property geoCoordinate startCentroid

        PinchHandler {
            id: pinch
            target: null
            onActiveChanged: if (active) {
                map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
            }
            onScaleChanged: (delta) => {
                map.zoomLevel += Math.log2(delta)
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            onRotationChanged: (delta) => {
                map.bearing -= delta
                map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
            }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
        WheelHandler {
            id: wheel
            // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
            // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
            // and we don't yet distinguish mice and trackpads on Wayland either
            acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                             ? PointerDevice.Mouse | PointerDevice.TouchPad
                             : PointerDevice.Mouse
            rotationScale: 1/120
            property: "zoomLevel"
        }
        DragHandler {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
        Shortcut {
            enabled: map.zoomLevel < map.maximumZoomLevel
            sequence: StandardKey.ZoomIn
            onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
        }
        Shortcut {
            enabled: map.zoomLevel > map.minimumZoomLevel
            sequence: StandardKey.ZoomOut
            onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
        }



    }
    function setCenterPosition(lati , longi)
    {
        map.pan(latitude-lati, longitude-longi)
        latitude = lati
        longitude = longi
    }
    function setLocationMarker(lati, longi)
    {
        var item = locationMarker.createObject(window, {coordinate: QtPositioning.coordinate(lati, longi)})
        map.addMapItem(item)
    }
    Component
    {
        id: locmarker
        MapQuickItem
        {
            id: markerImg
            anchorPoint.x: image.width
            anchorPoint.y: image.height
            coordinate: position
            zoomLevel: map.zoomLevel

            sourceItem: Image{

                    id: image
                    width: 20
                    height: 20

                    source: "file:///C:/Users/dark_/Desktop/Projects ideas/point.png"

                }
        }
    }
    /*Map{
    id: mapview
    anchors.fill: parent
    center:QtPositioning.coordinate(latitude, longitude)
    zoomLevel: 15
    plugin: Plugin {
        id: googlemapview
        name: "osm"
        PluginParameter {
                    name: "osm.mapping.providersrepository.disabled"
                    value: "true"
                }
                PluginParameter {
                    name: "osm.mapping.providersrepository.address"
                    value: "http://maps-redirect.qt.io/osm/5.6/"
                }



    }

    }*/

}

