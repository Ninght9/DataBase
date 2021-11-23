<!DOCTYPE html>
<html lang="en">
<?php session_start(); ?>

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>store_info</title>
    <link rel="stylesheet" href="../../layui-v2.6.8/layui/css/layui.css">
    <link rel="stylesheet" href="../../css/homepage.css">
    <script src="../../layui-v2.6.8/layui/layui.js"> </script>
    <script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
</head>

<body>
    <table id="store_info"></table>
</body>
<script type="text/javascript">
    layui.use("table", function() {
        var table = layui.table;
        table.render({
            elem: "#store_info",
            url: "../../tempfiles/storeinfo.json",
            page: true,
            cols: [
                [{
                        field: 'store_id',
                        title: "商铺ID",
                        width: 100,
                        sort: true
                    },
                    {
                        field: 'store_name',
                        title: "商铺名",
                        width: 100,
                        sort: true
                    }
                ]
            ],
        })
    })
</script>

</html>
<?php
$servername = "localhost";
$username = "root";
$password = "@)))!!!^";
$dbname = 'canteens';
// 创建连接
$conn = new mysqli($servername, $username, $password, $dbname);
// 检测连接
if ($conn->connect_error) {
    die("连接失败: " . $conn->connect_error);
}
//echo "连接成功";
$user_id=$_SESSION['user_id'];

$sql = "select store.store_id,store.store_name from store,canteen,manager,canteen_store 
        where manager.user_id=$user_id
        and manager.manager_id=canteen.manager_id 
        and canteen.canteen_id=canteen_store.canteen_id 
        and store.store_id=canteen_store.store_id;";
$result = $conn->query($sql);
$num = mysqli_num_rows($result);
$file = '../../tempfiles/storeinfo.json';
unlink($file);
file_put_contents($file, '{"code":0,"msg":"","count":', FILE_APPEND);
file_put_contents($file, $num, FILE_APPEND);
file_put_contents($file, ',"data":[', FILE_APPEND);
while ($select = $result->fetch_assoc()) {
    file_put_contents($file, json_encode($select), FILE_APPEND);
    file_put_contents($file, ',', FILE_APPEND);
}
$text = file_get_contents($file);
$text_delete_lastchar = substr($text, 0, strlen($text) - 1);
file_put_contents($file, $text_delete_lastchar);
file_put_contents($file, ']}', FILE_APPEND);
?>