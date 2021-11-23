<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>食堂信息</title>

    <link rel="stylesheet" href="../../css/canteen_manage.css">
    <link rel="stylesheet" href="../../layui-v2.6.8/layui/css/layui.css">
    <script src="../../layui-v2.6.8/layui/layui.js"> </script>
    <script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
</head>

<body>

    <nav class="layui-layout-admin" style="height:60px;background:#393D49;">
        <h1 class="title">食堂信息</h1>
        <ul class="layui-nav layui-layout-right">
            <li class="layui-nav-item">
                <a href='../homepage/homepage.php'>返回
                </a>
            </li>
        </ul>
    </nav>

    <ul class="layui-nav layui-nav-tree layui-inline side_left">
        <li class="layui-nav-item layui-this">
            <a href="./canteen_manage.php">
                <i class="iconfont icon-shouye1"></i>
                菜品
            </a>
        </li>
        <li class="layui-nav-item layui-nav-itemed" id="changeToStore">
            <a href="javascript:;">
                <i class="iconfont icon-shouye1"></i>
                店铺
            </a>
        </li>
        <li class="layui-nav-item layui-nav-itemed" id="changeToOrder">
            <a href="javascript:;">
                <i class="iconfont icon-shouye1"></i>
                订单查看
            </a>
        </li>
    </ul>

    <iframe src="./canteen_cuisine.php" frameborder="0" class="iframe" id="contentIframe" width=600 height=450></iframe>
</body>

</html>
<script src="../../layui-v2.6.8/layui/layui.js"> </script>
<script src="../../jQuery-js/jquery-3.2.1.min.js"></script>
<script>
    $(function() {
        $('#changeToStore').on('click', function() {
            $(window.parent.document).find('#contentIframe').attr('src', './store_info.php')
            $(this).addClass('layui-this').siblings().removeClass('layui-this');
        })
    })
    $(function() {
        $('#changeToOrder').on('click', function() {
            $(window.parent.document).find('#contentIframe').attr('src', './orders.php')
            $(this).addClass('layui-this').siblings().removeClass('layui-this');
        })
    })
</script>