<!DOCTYPE html>
<html lang="en">
<?php session_start(); ?>

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>order</title>
    <link rel="stylesheet" href="../../layui-v2.6.8/layui/css/layui.css">
    <link rel="stylesheet" href="../../css/homepage.css">
    <script src="../../layui-v2.6.8/layui/layui.js"> </script>
    <script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
</head>

<body>
    <table id="order"></table>
    <div class="layui-btn-group demoTable">
        <button class="layui-btn" data-type="getCheckData">查看已选</button>
        <button class="layui-btn" data-type="getCheckLength">下单</button>
    </div>
</body>
<script type="text/javascript">
    layui.use("table", function() {
        var table = layui.table;
        table.render({
            elem: "#order",
            url: "../../tempfiles/order.json",
            page: true,
            cols: [
                [{
                        field: 'aa',
                        type: "checkbox"
                    },
                    {
                        field: '',
                        title: "这里是图片",
                        width: 145,
                    },
                    {
                        field: 'canteen_id',
                        title: "食堂ID",
                        width: 100,
                        sort: true
                    },
                    {
                        field: 'cuisine_name',
                        title: "菜品",
                        width: 150,
                        sort: true
                    },
                    {
                        field: 'cost',
                        title: "价格",
                        width: 145,
                        sort: true
                    }
                ]
            ],
        })
    })
</script>
<script>
    layui.use('table', function() {
        var table = layui.table;
        var $ = layui.$,
            active = {
                getCheckData: function() { //获取选中数据
                    var checkStatus = table.checkStatus('order'),
                        data = checkStatus.data;
                    layer.alert(JSON.stringify(data));
                },
                getCheckLength: function() { //下单
                    var checkStatus = table.checkStatus('order'),
                        data = checkStatus.data;
                    layer.msg('下单成功');
                    var datas = JSON.stringify(data);
                    var test = "asd";
                    // $.ajaxSettings.async = false;
                    $.post("./order.php", {
                        data: test
                    });
                }
            };

        $('.demoTable .layui-btn').on('click', function() {
            var type = $(this).data('type');
            active[type] ? active[type].call(this) : '';
        });
    });
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
// echo "连接成功";
$user_id = $_SESSION['user_id'];
if ($user_id) {
    $sql = "select canteen_id,cuisine_name,cost from canteen_cuisine,cuisine where
        canteen_cuisine.cuisine_id=cuisine.cuisine_id;";
    $result = $conn->query($sql);
    $num = mysqli_num_rows($result);
    $file = '../../tempfiles/order.json';
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
}

$info = "123";

// $info = $_POST['data'];
$info = json_decode($_POST["data"], true);

?>