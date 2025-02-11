$Id: README.txt,v 1.1 2010/03/01 23:07:49 jcarnett Exp $

CSS Embedded Images for Drupal 6
==================================

The CSS Embedded Images module for Drupal 6 will replace image URLs in
aggregated CSS files with data URI schemes when CSS optimization is enabled.
This can greatly reduce the number of HTTP requests required to load your
pages in browsers that support embedded images in CSS:

 * Firefox 2+
 * Safari
 * Google Chrome
 * Opera 7.2+
 * Internet Explorer 8+


Installation
------------

 1) Copy the CSS Embedded Images module to sites/all/modules.
 
 2) Enable it in admin/build/modules.
 
 3) Enable CSS Optimization in admin/settings/performance.
 
 4) See that the aggregated CSS file now ends with ".emimage.css",
    and contains embedded images.
