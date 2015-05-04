#GDES
瓦斯抽采达标评价系统<br>
两台电脑同步测试

<br>
参考资料：<br>
##github fork使用方法
来源：http://www.cnblogs.com/shiningrise/archive/2013/01/14/2860248.html

概要： 克隆别人的代码库到自己的项目中，可以作为子模块的形式使用，或二次开发 

操作流程： 
在开源项目中点击fork按钮，稍等一会儿，该项目便会拷贝一份到你的respositories中， 
克隆一份代码到本地：git clone git@github.com:username/Spoon-Knife.git 

配置：（项目克隆完成后，默认远程的别名为origin，此为我们自己项目中的版本，并非原始作者的代码库） 

创建原始代码库的别名，方便跟踪代码
git remote add upstream git://github.com/octocat/Spoon-Knife.git 
git fetch upstream 
git merge upstream/master 

如果你希望将自己的代码贡献到原始代码库中，可参见http://help.github.com/send-pull-requests/ 来完成