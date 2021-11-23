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
    <table id="user_info"></table>
</body>
<script type="text/javascript">
    layui.use("table", function() {
        var table = layui.table;
        table.render({
            elem: "#user_info",
            url: "../../tempfiles/userinfo.json",
            cols: [
                [{
                        field: 'user_id',
                        title: "账号",
                    },
                    {
                        field: 'user_name',
                        title: "用户名",

                    },
                    {
                        field: 'user_type',
                        title: "用户类型",

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

$sql = "select * from user 
        where user_id=$user_id";
$result = $conn->query($sql);
$num = mysqli_num_rows($result);
$file = '../../tempfiles/userinfo.json';
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