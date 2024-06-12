import React, { useState, useEffect, useRef } from "react";

export default function Map(props) {

  let prevMarkersRef = useRef([]);
  let prevCirclesRef = useRef([]);

  useEffect(() => {
    const google = window.google;

    // Create an instance of the map
    const uluru = new google.maps.LatLng(25.0232322, 121.5369097);
    // The map, centered at Uluru
    const cur_map = build_interactive_map(uluru);

    clearMarkers(prevMarkersRef.current);

  }, [])// eslint-disable-line react-hooks/exhaustive-deps

  let createMarker = (latlng, map) => {
    return new window.google.maps.Marker({
      position: latlng,
      map: map,
      draggable:false,
      icon: 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/blue%20(1).png'
    });
  }

  let marker_level = (latlng, map, tent) => {
    var marker_icon;
    if (tent >= 0 && tent <15.5){
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/green%20(1).png';
    } else if (tent >= 15.5 && tent < 35.5) {
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/yellow%20(2).png';
    } else if (tent >= 35.5 && tent < 54.5){
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/orange%20(1).png';
    } else if (tent >=54.5 && tent < 150.5){
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/red%20(1).png';
    } else {
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/purple%20(1).png';
    }
   return new window.google.maps.Marker({
      position: latlng,
      map: map,
      draggable: false,
      icon: marker_icon
    });
  }

  let uav_marker = (latlng, map, tent) => {
    var marker_icon;
    if (tent >= 0 && tent <15.5){
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/uav_g.png';
    } else if (tent >= 15.5 && tent < 35.5) {
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/uav_y.png';
    } else if (tent >= 35.5 && tent < 54.5){
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/uav_o.png';
    } else if (tent >=54.5 && tent < 150.5){
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/uav_r.png';
    } else {
      marker_icon = 'https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/uav_p.png';
    }
   return new window.google.maps.Marker({
      position: latlng,
      map: map,
      draggable: false,
      icon: marker_icon
    });
  }

  // Removes the markers from the map.
  let clearMarkers = (markers) => {
    for (let m of markers) {
      m.setMap(null);
    }
  }

  let createCircle = (latlng, map) => {
    return new window.google.maps.Circle({
      strokeColor: "#FF0000",
      strokeOpacity: 0.8,
      strokeWeight: 2,
      fillColor: "#FF0000",
      fillOpacity: 0.35,
      map: map,
      center: latlng,
      radius: 100,
      editable: true,
    });
  }

  let clearCircles = (circles) => {
    for (let c of circles) {
      c.setMap(null);
    }
  }

  let build_interactive_map = (latlng, zoom = 14) => {
    const google = window.google;
    const cur_map = new google.maps.Map(document.getElementById("map"), {
      zoom: zoom,
      center: latlng,
    });
    
    addMapListener(cur_map)

    return cur_map;
  }

  let pm25_info = (marker, content) => {
    const google = window.google;
    var a = -1;
    var pm_25 = new google.maps.InfoWindow({
        content: 'PM2.5：'+ content +'μg/m3'
    });
    google.maps.event.addListener(marker, "click", ()=>{
      a = a*-1;
      if(a > 0){
        pm_25.open(marker.get('map'), marker);
      }else{
        pm_25.close();
      }
    });
  }

  let uav_pm25_info = (marker, pm, height) => {
    const google = window.google;
    var a = -1;
    var pm_25 = new google.maps.InfoWindow({
        content:"<p>"+'PM2.5：'+ pm +'μg/m3'+ "<br />" + '高度：'+ height+'公尺'+"</p>"
    });
    google.maps.event.addListener(marker, "click", ()=>{
      a = a*-1;
      if(a > 0){
        pm_25.open(marker.get('map'), marker);
      }else{
        pm_25.close();
      }
    });
  }

  let circle_range = (lat, lng, radius, map) =>{
    const url = "http://localhost:3001/compare_radius";
    var data = {
      'coordinates': [parseFloat(lng), parseFloat(lat)],
      'radius': parseFloat(radius)
    }
    fetch(url, {
      method: "POST",
      body: JSON.stringify(data),
      headers: new Headers({
        "Content-Type": "application/json"
      })
    })
    .then((res) => {
      return res.json();
    })
    .catch((error) => console.error("Error:", error))
    .then((response) => {
      console.log(response)
      if (response.length > 0){
        for(var i = 0; i < response.length; i++){
           var obj = JSON.parse(response[i].point)
           var lng = obj.coordinates[0]
           var lat = obj.coordinates[1]
           var tent = response[i].p_pm25;
           const in_range_marker = marker_level({lat: lat, lng: lng}, map, tent)
           pm25_info(in_range_marker,tent)
           prevMarkersRef.current.push(in_range_marker);
        }
      }
    });
  }

  let uav_pm = (lat, lng, radius, map) =>{
    const url = "http://localhost:3001/uav_values";
    var data = {
      'coordinates': [parseFloat(lng), parseFloat(lat)],
      'radius': parseFloat(radius)
    }
    fetch(url, {
      method: "POST",
      body: JSON.stringify(data),
      headers: new Headers({
        "Content-Type": "application/json"
      })
    })
    .then((res) => {
      return res.json();
    })
    .catch((error) => console.error("Error:", error))
    .then((response) => {
      console.log(response)
      if (response.length > 0){
        for(var i = 0; i < response.length; i++){
           var obj = JSON.parse(response[i].point)
           var lng = obj.coordinates[0]
           var lat = obj.coordinates[1]
           var pm = response[i].p_pm25;
           var hei = response[i].height;
           const in_range_marker = uav_marker({lat: lat, lng: lng}, map, pm)
           uav_pm25_info(in_range_marker,pm,hei)
           prevMarkersRef.current.push(in_range_marker);
        }
      }
    });
  }

  var infowindow = new window.google.maps.InfoWindow({
    content: '<h1>Origin</h1>'
  });

  let addMapListener = (map) => {
    const google = window.google;
    var a = -1;
    map.addListener("click", (e) => {
      const marker = createMarker(e.latLng, map);
      const cir = createCircle(e.latLng, map);

      marker.addListener("click", ()=>{
            a = a*-1;
            if(a > 0){
                infowindow.open(map, marker);
            }else{
                infowindow.close();
            }
      });
      
      google.maps.event.addListener(cir, "radius_changed", () => {
        circle_range(e.latLng.lat(),e.latLng.lng(),cir.getRadius(),map)
        uav_pm(e.latLng.lat(),e.latLng.lng(),cir.getRadius(),map)
      });

      clearMarkers(prevMarkersRef.current);
      clearCircles(prevCirclesRef.current);
      prevMarkersRef.current.push(marker);
      prevCirclesRef.current.push(cir);
    });
  }
  
  return (
    // Set a height on the map so it will display
    <>
      <div>
        <div id="map" style={{ height: "700px" }} />
      </div>
    </>
  );
}