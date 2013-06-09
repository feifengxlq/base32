--TEST--
Test base32_decode() function : basic functionality - ensure all base32 alphabet is supported.
--FILE--
<?php
$bolRet = true;
for($i=0; $i<255; ++$i) {
	$strD = base32_decode(base32_encode(pack("C", $i)));
	$arrR = unpack("C", $strD);
	if ($arrR[1] !== $i) {
		$bolRet = false;
		break;
	}
}
echo intval($bolRet);
?>
--EXPECT--
1
