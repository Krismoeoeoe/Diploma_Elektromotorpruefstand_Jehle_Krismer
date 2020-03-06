// author: thetoastiestbread
// date: 01.03.2020
// filename: JScode_V1.js

function getStuff(){
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function(){
		if(this.readyState == 4 && this.status == 200){
			var myObj = JSON.parse(this.responseText);
			document.getElementById("Ergs").innerHTML = myObj;
		}
	};
	xmlhttp.open("GET", "Stuff/PHPgetfromFile_V1.php", true);
	xmlhttp.send();
}