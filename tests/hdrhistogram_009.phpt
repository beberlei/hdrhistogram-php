--TEST--
hdrhistogram: 
Author: kontakt@beberlei.de
--FILE--
<?php

$num = 1000 * 60;
$hdr = hdr_init(1, $num, 2);

hdr_record_values($hdr, 1, 10);
hdr_record_values($hdr, 2, 20);
hdr_record_values($hdr, 3, 25);
hdr_record_values($hdr, 10, 10);
hdr_record_values($hdr, 20, 5);
hdr_record_values($hdr, 30, 2);
hdr_record_values($hdr, 40, 1);

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
p(50)=3
p(95)=20
HISTggAAAFp4nO3MsQmAMBRF0W/AxtpGcIBs4SquIriBW1o5gkVSikGwPKe5zePN+7FF5BxFqu1KznW54slQO9ZOj6u2oT35pP/5L7Unn3Q//wEAAAAAAAAA
p(50)=3
p(95)=20
Buckets: 41
array(7) {
  [0]=>
  int(34)
  [10]=>
  int(2)
  [20]=>
  int(1)
  [25]=>
  int(1)
  [5]=>
  int(1)
  [2]=>
  int(1)
  [1]=>
  int(1)
}
