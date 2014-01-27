Overview
--------

Bad Behavior is a set of PHP scripts which prevents spambots from
accessing your site by analyzing their actual HTTP requests and
comparing them to profiles from known spambots. It goes far beyond
User-Agent and Referer, however.

The problem: Spammers run automated scripts which read everything on
your web site, harvest email addresses, and if you have a blog, forum
or wiki, will post spam directly to your site. They also put false
referrers in your server log trying to get their links posted through
your stats page.

As the operator of a Web site, this can cause you several
problems. First, the spammers are wasting your bandwidth, which you
may well be paying for. Second, they are posting comments to any form
they can find, filling your web site with unwanted (and unpaid!) ads
for their products. Last but not least, they harvest any email
addresses they can find and sell those to other spammers, who fill
your inbox with more unwanted ads.

Bad Behavior intends to target any malicious software directed at a
Web site, whether it be a spambot, ill-designed search engine bot, or
system crackers.


Requirements
------------

- Drupal 6.x
- PHP 4.3.0 or greater
- BadBehavior 2.1.13 (http://www.bad-behavior.ioerror.us/download/)


Installation
------------

1. Extract the tarball into the modules folder of your Drupal install.

2. Download the BadBehavior 2.1.13 version PHP scripts from
   http://www.bad-behavior.ioerror.us/download/, unzip it,
   and copy the bad-behavior directory into your local
   sites/all/libraries/ directory.

   Here are the recommended steps to do this from the command line:

   cd /[path/to/site]/sites/all/libraries/
   wget http://downloads.wordpress.org/plugin/bad-behavior.2.1.13.zip
   unzip bad-behavior.2.1.13.zip
   rm bad-behavior.2.1.13.zip

3. Enable the module as usual from the Drupal Admin > Modules page.


Compatibility Notes
-------------------

1. When using this module with Boost module enabled, you must have a
   whitelist.ini file in the BB script directory. A blank one can be
   created using the following from the command line:
   touch /[path/to/site]/sites/all/libraries/bad-behavior/whitelist.ini
   If this file doesn't exist while using Boost module, Boost will write
   file-not-found errors in the server logs.


Configuration
-------------

1. If desired, configure settings under the new
   Admin > Settings > Bad Behavior menu item.


Logs
----

1. View BadBehavior logs at the new Admin > Reports > Bad Behavior menu item.

2. Click on the detail link next to any log item for full details.


Frequently Asked Questions
--------------------------

See: http://www.bad-behavior.ioerror.us/faq/


