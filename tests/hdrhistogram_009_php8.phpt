--TEST--
hdrhistogram: Base64 Encode/Decode
--SKIPIF--
<?php
if (PHP_VERSION_ID < 80000) die("skip PHP 8 only");
if (getenv('SKIP_ASAN')) die("xfail https://github.com/HdrHistogram/HdrHistogram_c/issues/123");
?>
--FILE--
<?php

$a = hdr_init(1, 100, 1);

for ($i = 0; $i < 100; $i++) {
    hdr_record_values($a, $i, 1);
}

$data = hdr_base64_encode($a);
$hdr = hdr_base64_decode($data);

$iterator = hdr_iter_init($hdr);
$total = 0;
while ($data = hdr_iter_next($iterator)) {
    if ($data['count_at_index']) {
        echo $data['value'] . ': ' . $data['count_at_index'] . "\n";
        $total += $data['count_at_index'];
    }
}
echo "Total: $total\n";

try {
    echo hdr_base64_encode("foo");
} catch (TypeError $e) {
	echo $e->getMessage()."\n";
}
echo hdr_base64_decode("foo");

echo hdr_max(hdr_base64_decode(hdr_base64_encode(hdr_init(1, 100, 1))));

?>
--EXPECTF--
0: 1
1: 1
2: 1
3: 1
4: 1
5: 1
6: 1
7: 1
8: 1
9: 1
10: 1
11: 1
12: 1
13: 1
14: 1
15: 1
16: 1
17: 1
18: 1
19: 1
20: 1
21: 1
22: 1
23: 1
24: 1
25: 1
26: 1
27: 1
28: 1
29: 1
30: 1
31: 1
32: 2
34: 2
36: 2
38: 2
40: 2
42: 2
44: 2
46: 2
48: 2
50: 2
52: 2
54: 2
56: 2
58: 2
60: 2
62: 2
64: 4
68: 4
72: 4
76: 4
80: 4
84: 4
88: 4
92: 4
96: 4
Total: 100
hdr_base64_encode(): Argument #1 ($hdr) must be of type resource, string given

Warning: hdr_base64_decode(): Cannot decode histogram in %s on line %d
0
