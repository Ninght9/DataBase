<!DOCTYPE html>
<html lang="en">
<?php session_start()?>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="../../layui-v2.6.8/layui/css/layui.css">
    <link rel="stylesheet" href="../../css/homepage.css">
    <script src="../../layui-v2.6.8/layui/layui.js"> </script>
    <script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
    <title>orders</title>
</head>

<body>
    <table id="demo"></table>
</body>

</html>
<script type="text/javascript">
    layui.use("table", function() {
        var table = layui.table;
        table.render({
            elem: "#demo",
            url: "../../tempfiles/orders.json",
            page: true,
            cols: [
                [{
                        field: 'canteen_id',
                        title: "食堂id",
                        width: 100,
                        sort: true
                    },
                    {
                        field: 'user_id',
                        title: "用户id",
                        width: 100,
                        sort: true
                    },
                    {
                        field: 'cuisine_name',
                        title: "菜品",
                        width: 100,
                        sort: true
                    },
                    {
                        field: 'amount',
                        title: "消费",
                        width: 120,
                        sort: true
                    }
                ]
            ],
        })
    })
</script>

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
$sql = "select canteen_cuisine.canteen_id,cuisine_name,amount,orders.user_id from orders,canteen_cuisine,cuisine,manager,canteen where
        manager.user_id='111' and
        manager.manager_id=canteen.manager_id and
        orders.canteen_id=canteen.canteen_id and
        orders.cuisine_id=canteen_cuisine.cuisine_id and
        cuisine.cuisine_id=orders.cuisine_id and
        orders.canteen_id=canteen_cuisine.canteen_id ";
$result = $conn->query($sql);
$num = mysqli_num_rows($result);
$file = '../../tempfiles/orders.json';
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