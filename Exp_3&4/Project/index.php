<!DOCTYPE html>
<?php
session_start();
?>
<html lang="en">

<head>

    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>欢迎，请先登录！</title>
    <link rel="stylesheet" href="./layui-v2.6.8/layui/css/layui.css" media="all">
    <script src="/layui-v2.6.8/layui/layui.js" charset="utf-8"></script>
</head>

<body>
    <form action="./index.php" class="layui-form" method="post">
        <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            登录</h2>
        <div class="layui-form-item">
            
            <label class="layui-form-label">账号</label>
            <div class="layui-input-inline">
                <input type="text" name="userid" lay-verify="required" autocomplete="off" placeholder="请输入账号" class="layui-input">
            </div>
        </div>
        <div class="layui-form-item">
            <label class="layui-form-label">密码</label>
            <div class="layui-input-inline">
                <input type="password" name="password" lay-verify="required" placeholder="请输入" autocomplete="off" class="layui-input">
            </div>
        </div>
        <div class="layui-form-item">
            <div class="layui-input-block">
                <button class="layui-btn" type="submit" lay-filter="demo1">登录</button>
                <button type="reset" class="layui-btn layui-btn-primary">重置</button>
            </div>
        </div>
        <div class="layui-form-item" >
            <a href="php/register.php">
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            注册</a>
        </div>
    </form>
</body>

</html>
<?php
//error_reporting(0);
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
$login_userid = $_POST['userid'];
$login_password = $_POST['password'];
$sql_login = "select * from user where user_id='$login_userid' and password='$login_password'";
$result_login = $conn->query($sql_login);
$login = $result_login->fetch_assoc();
if (!$result_login) {
    printf("Error: %s\n", mysqli_error($conn));
    echo "1";
    exit(1);
} else if ($login_userid != '' && $login_password != '') {
    //echo "1";
    $number_login = mysqli_num_rows($result_login);

    if ($number_login == 1) {
        $_SESSION["user_id"] = $login_userid;
        if ($login['user_type'] == 'manager') {
            //echo"manager";
            echo "<script>window.location.href = 'php/homepage/homepage.php';</script>";
            exit(0);
        } else if ($login['user_type'] == 'customer') {
            echo "<script>window.location.href = 'php/homepage/homepage_customer.php';</script>";
            exit(0);
        } else {
            echo "<script>alrt'账号或密码错误';</script>";
        }
    } else {
        echo "<script>alrt'账号或密码错误';</script>";
        exit(0);
    }
}
?>