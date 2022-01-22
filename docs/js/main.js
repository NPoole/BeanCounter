document.getElementById("viewBody").onscroll = function() {
    var offset = 0;
    var animMargin = window.innerHeight-(window.innerHeight*0.1);
    var chunkTop = document.getElementById("chunkFour").getBoundingClientRect().top;
    if(chunkTop < animMargin){
        offset = animMargin-chunkTop;
    }
    document.getElementById("bcLogoCover").style.marginTop = (offset/8) + "px";
    document.getElementById("bcLogoCover").style.marginLeft = (offset/8) + "px";

    document.getElementById("bcThinSpacerC").style.marginTop = (offset/16) + "px";
    document.getElementById("bcThinSpacerC").style.marginLeft = (offset/16) + "px";
    
    document.getElementById("bcSegmentSpacer").style.marginTop = "-"+(offset/16) + "px";
    document.getElementById("bcSegmentSpacer").style.marginLeft = "-"+(offset/16) + "px";
    
    document.getElementById("bcEndSpacer").style.marginTop = "-" + (offset/16) + "px";
    document.getElementById("bcEndSpacer").style.marginLeft = "-" + (offset/16) + "px";
    
    document.getElementById("bcMidSpacer").style.marginTop = "-" + (offset/16) + "px";
    document.getElementById("bcMidSpacer").style.marginLeft = "-" + (offset/16) + "px";
    
    document.getElementById("bodyImg").style.marginTop = "-" + (offset/8) + "px";
    document.getElementById("bodyImg").style.marginLeft = "-" + (offset/8) + "px";
    document.getElementById("bodyImg").style.paddingRight = (offset/8) + "px";

    document.getElementById("bcBody").style.marginLeft = (offset/16) + "px";

    document.getElementById("bcTape").style.top = document.getElementById("chunkThree").getBoundingClientRect().top + "px";

};

document.getElementById("bcTape").style.top = document.getElementById("chunkThree").getBoundingClientRect().top + "px";