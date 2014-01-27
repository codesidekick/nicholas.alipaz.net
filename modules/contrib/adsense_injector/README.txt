// $Id: README.txt,v 1.1.4.3.2.3 2009/01/02 04:28:02 hswong3i Exp $
Copyright 2006-2011 Exodus Development, Inc. (http://exodusdev.com)

ABOUT ADSENSE CONTENT INJECTOR
------------------------------

The purpose of this module is to provide a simple method of automatically
placing 'inline' ads or other content into selected node types. It does
this by intercepting node rendering using hook_nodeapi during 'page' rendering,
and injecting selected text into the $node->body field.  It is NOT an
input filter but performs some of the same functions.

Official Adsense Injector project page:
  http://exodusdev.com/drupal/4.7/modules/adsense_injector.module

Drupal Adsense Injector project page:
  http://drupal.org/project/adsense_injector

Sponsored by:
  http://exodusdev.com/


RATIONALE
----------------------

Why is this useful? In my experience, this simplifies certain important
aspects of ad insertion and placement.

Traditional approaches:

  - Modify your theme's node.tpl.php or other template file(s) in order to
    inject ads on every node view.

    What happens if you have multiple sites or use multiple themes, or use
    custom per-node-type template files (node-book.tpl.php, node-image.tpl.php
    etc)? Now you have to edit, test, and maintain multiple template files,
    and, if the theme is updated to fix bugs, you have to merge in your
    changes.

  - Hand-edit each node content and use inline [adsense:x:y] inline filter
    tags.

    This gives tremendous flexibility in layout, but creates a maintenance
    nightmare if you should wish to alter your channel or ad layouts site-wide.

  - Use block insertion into the theme's template regions.

    This is great if your theme's regions provide the flexibility you want - it
    seems that themes vary somewhat in the regions they provide, and those
    regions aren't always in the places you want - so once again, you are back
    to tweaking theme template files if you want to place the ads near or in
    the content.


LIST OF MAINTAINERS
----------------------

PROJECT OWNER
M: Michael Curry <exodusdev@gmail.com>
S: maintained
W: http://exodusdev.com/


