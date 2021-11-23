<!DOCTYPE html>
<?php session_start() ?>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <link rel="stylesheet" href="../../layui-v2.6.8/layui/css/layui.css">
    <link rel="stylesheet" href="../../css/homepage.css">
    <script src="../../layui-v2.6.8/layui/layui.js"> </script>
    <script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
</head>

<body>
    <nav class="layui-layout-admin" style="height:60px;background:#393D49;">
        <h1 class="title">欢迎！</h1>
        <ul class="layui-nav layui-layout-right">
            <li class="layui-nav-item">
                <a href='../homepage/homepage.php'>返回
                </a>
            </li>
        </ul>
    </nav>

    <ul class="layui-nav layui-nav-tree layui-inline side_left">
        <li class="layui-nav-item layui-this" id="changeToCuisine">
            <a href="userinfo.php">
                <i class="iconfont icon-shouye1"></i>
                个人信息
            </a>
        </li>
    </ul>
</body>
<iframe src="./getinfo.php" frameborder="0" id="contentIframe" width=600 height=450></iframe>

</html>
