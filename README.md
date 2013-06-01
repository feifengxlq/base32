base32
======

 php extension,  base32 encoding / decoding 


INSTALL
====

/path/to/php/bin/phpize

./configure --enable-base32 --with-php-config=/path/to/php/bin/php-config

make 

make intall

vim php.ini && enable base32.so

TEST
====

/path/to/php/bin/php -r "var_dump(base32_decode(base32_encode('1234567890')));"
