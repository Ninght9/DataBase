<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>homepage</title>
    <link rel="stylesheet" href="../../layui-v2.6.8/layui/css/layui.css">
    <link rel="stylesheet" href="../../css/homepage.css">
</head>

<body>
    <nav class="layui-layout-admin" style="height:60px;background:#393D49;">
        <h1 class="title">欢迎！</h1>
        <ul class="layui-nav layui-layout-right">
            <li class="layui-nav-item">
                <a href="../canteen_manage/canteen_manage.php">食堂信息
                </a>
            </li>
            <li class="layui-nav-item">
                <a href="../userinfo/userinfo.php">个人信息
                </a>
            </li>
            <li class="layui-nav-item">
                <a href='../../index.php'>退出
                </a>
            </li>
        </ul>
    </nav>

    <ul class="layui-nav layui-nav-tree layui-inline side_left">
        <li class="layui-nav-item layui-this" id="changeToCuisine">
            <a href="../homepage/homepage.php">
                <i class="iconfont icon-shouye1"></i>
                点餐
            </a>
        </li>
        <li class="layui-nav-item layui-nav-itemed" id="changeToMyorder">
            <a href="javascript:;">
                <i class="iconfont icon-shouye1"></i>
                我的订单
            </a>
        </li>
    </ul>


    <iframe src="./order.php" frameborder="0" id="contentIframe" width=600 height=450></iframe>


</body>

</html>
<script src="../../layui-v2.6.8/layui/layui.js"> </script>
<script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
<script>
    $(function() {
        $('#changeToMyorder').on('click', function() {
            $(window.parent.document).find('#contentIframe').attr('src', './myorders.php')
            $(this).addClass('layui-this').siblings().removeClass('layui-this');
        })
    })
</script>