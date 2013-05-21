var Input, provFavicon, disp;

self.port.on("show", function (searchEngine, icon1) {
  Input = document.getElementById('search-box');
  provFavicon = document.getElementById('favicon');
  Input.focus();
  Input.onkeyup = function(event) {
    if (event.keyCode == 13) {
      // Remove the newline.
      text = Input.value.replace(/(\r\n|\n|\r)/gm,"");
      self.port.emit("text-entered", text);
      Input.value = '';
    }
  };
  
  if (icon1 == null | icon1 == undefined| icon1 == '') {
      icon1 = 'style/search.png';      
  }
  provFavicon.src = icon1;
  if (searchEngine == "" | searchEngine == null | searchEngine == undefined) {
      searchEngine = "Search";
  }
  Input.placeholder = searchEngine;
  
});