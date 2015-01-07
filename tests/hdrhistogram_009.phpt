--TEST--
hdrhistogram: 
Author: kontakt@beberlei.de
--FILE--
<?php

$num = 1000 * 60;
$hdr = hdr_init(1, $num, 2);

for ($i = 0; $i < ($num - 1); $i++) {
    $len = strlen((string)$num);
    hdr_record_values($hdr, $i+1, 6 - $len);
}

$data = hdr_log_encode($hdr);
echo "p(50)=" . hdr_value_at_percentile($hdr, 50) . "\n";
echo "p(95)=" . hdr_value_at_percentile($hdr, 95) . "\n";

echo $data . "\n";

$hdr_decoded = hdr_log_decode($data);
echo "p(50)=" . hdr_value_at_percentile($hdr_decoded, 50) . "\n";
echo "p(95)=" . hdr_value_at_percentile($hdr_decoded, 95) . "\n";

$iter = hdr_iter_init($hdr_decoded);
$buckets = array();
$counts = array();

while ($data = hdr_iter_next($iter)) {
    $buckets[] = $data;

    if (!isset($counts[$data['count_at_index']])) {
        $counts[$data['count_at_index']] = 0;
    }

    $counts[$data['count_at_index']]++;
}

echo "Buckets: " . count($buckets) . "\n";
var_dump($counts);

--EXPECTF--
p(50)=30079
p(95)=57087
HISTggAAAFl4nO3FsQ1AUBQAwPdFoTSAQmkLRjEKiQ1sqTKCglKpkXfXXLfta8QwxK16LnfHPJ7xpti27TRXttNe2057Yzvtre2097bTPtlO+2I76yVsf3U=
p(50)=30079
p(95)=57087
Buckets: 1258
array(10) {
  [0]=>
  int(1)
  [1]=>
  int(255)
  [2]=>
  int(128)
  [4]=>
  int(128)
  [8]=>
  int(128)
  [16]=>
  int(128)
  [32]=>
  int(128)
  [64]=>
  int(128)
  [128]=>
  int(128)
  [256]=>
  int(106)
}
