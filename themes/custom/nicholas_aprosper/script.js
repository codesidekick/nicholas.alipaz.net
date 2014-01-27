$(document).ready(function() {
  // set all links with rel="blank" to open in target _blank
  $('a[rel="blank"],a[rel="_blank"],a[rel="external"],a.blank,a.external').click(function(){
    this.target = "_blank";
  });
});
