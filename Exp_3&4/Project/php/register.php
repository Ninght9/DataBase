<!DOCTYPE html>

<html lang="en">

<head>

    <meta charset="UTF-8">
    <title>注册</title>
    <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
    <link rel="stylesheet" href="../layui-v2.6.8/layui/css/layui.css" media="all">
    <script src="../layui-v2.6.8/layui/layui.js" charset="utf-8"></script>
</head>

<body>
    <form action="./register.php" class="layui-form" method="post">
        <h2>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注册</h2>
        <div class="layui-form-item">
            <label class="layui-form-label">用户名</label>
            <div class="layui-input-inline">
                <input type="text" name="username" lay-verify="required" autocomplete="off" placeholder="请输入用户名" class="layui-input">
            </div>
        </div>
        <div class="layui-form-item">
            <label class="layui-form-label">账号</label>
            <div class="layui-input-inline">
                <input type="text" name="userid" lay-verify="required" autocomplete="off" placeholder="请输入账号" class="layui-input">
            </div>
        </div>
        <div class="layui-form-item">
            <label class="layui-form-label">密码</label>
            <div class="layui-input-inline">
                <input type="password" name="password" lay-verify="required" placeholder="请输入密码" autocomplete="off" class="layui-input">
            </div>
        </div>
        <div class="layui-form-item">
            <label class="layui-form-label">身份</label>
            <div class="layui-input-block">
                <input type="radio" name="user_type" value="manager" title="管理员" checked="">
                <input type="radio" name="user_type" value="customer" title="学生">

            </div>
        </div>
        <div class="layui-form-item">
            <div class="layui-input-block">
                <button class="layui-btn" type="submit" lay-filter="demo1">注册</button>
                <button type="reset" class="layui-btn layui-btn-primary">重置</button>
            </div>
        </div>
        <div class="layui-form-item" >
            <a href="../index.php">
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            返回登录</a>
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
$register_username = $_POST['username'];
$register_userid = $_POST['userid'];
$register_password = $_POST['password'];
$register_usertype = $_POST['user_type'];
$sql_check = "select * from user where user_id='$register_userid'";
$result_check = $conn->query($sql_check);
if (!$result_check) {
    printf("Error: %s\n", mysqli_error($conn));
    exit();
} else {
    $number_check = mysqli_num_rows($result_check);
    if ($number_check) {
        echo "<script>alert('账号已存在！');</script>";
        exit(0);
    } else if ($register_password != '' && $register_userid != '' && $register_username != '' && $register_usertype != '') {
        $sql_insert = "insert into user values('$register_userid','$register_username','$register_usertype','$register_password')";
        $result_insert = $conn->query($sql_insert);
        echo "<script>alert('注册成功！');windows.location.href='index.php';</script>";
        exit(0);
    }
}

?>