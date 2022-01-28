$("#viewBody").scroll(function() {
    var offset = 0;
    var animMargin = $("#viewBody").height() - ($("#viewBody").height() * 0.1);
    var chunkTop = $("#chunkFour")[0].getBoundingClientRect().top;
    if (chunkTop < animMargin) {
        offset = ((1 - (chunkTop / animMargin)) * ($("#bcBody").height() / 50));
    }

    $("#bodyImg").css("marginTop", "-" + (offset * 10) + "px");
    $("#bodyImg").css("marginLeft", "-" + (offset * 10) + "px");

    $("#bcEndSpacer").css("marginTop", (offset * 10) + "px");
    $("#bcEndSpacer").css("marginLeft", (offset * 10) + "px");

    $("#bcMidSpacer").css("marginTop", (offset * 10) + "px");
    $("#bcMidSpacer").css("marginLeft", (offset * 10) + "px");

    $("#bcSegmentSpacer").css("marginTop", (offset * 10) + "px");
    $("#bcSegmentSpacer").css("marginLeft", (offset * 10) + "px");

    $("#bcThinSpacerA").css("marginTop", (offset * 30) + "px");
    $("#bcThinSpacerA").css("marginLeft", (offset * 30) + "px");

    $("#bcThinSpacerB").css("marginTop", (offset * 30) + "px");
    $("#bcThinSpacerB").css("marginLeft", (offset * 30) + "px");
    $("#bcThinSpacerB").css("marginRight", "-" + (offset * 30) + "px");

    $("#bcThinSpacerC").css("marginTop", (offset * 50) + "px");
    $("#bcThinSpacerC").css("marginLeft", (offset * 50) + "px");

    $("#bcLogoCover").css("marginTop", (offset * 70) + "px");
    $("#bcLogoCover").css("marginLeft", (offset * 70) + "px");

    $("#bcBody").css("marginLeft", (offset * 0) + "px");

    $("#bcTape").css("top", $("#chunkThree")[0].getBoundingClientRect().top + "px");

});

$(window).resize(function() {
    $(".sectionHeader").each(function() {
        var divRatio = $(this).width() / $(this).height();
        $(this).find(".areaFit").css("font-size", (1 + (divRatio / 2)) * 2 + "vh");
    });
    $(".sectionBody").each(function() {
        var divRatio = $(this).width() / $(this).height();
        $(this).find(".areaFit").css("font-size", (1 + (divRatio / 2)) * 1.4 + "vh");
    });
    var screenRatio = $(window).width() / $(window).height();
    if (screenRatio > 1.33) {
        var padding = ($(window).width() - ($(window).height() * 1.33)) / 2;
        $("#viewBody").css("padding-left", padding + "px");
        $("#viewBody").css("padding-right", padding + "px");
        $("#bcBody").css("left", (($("#viewBody").width() * 0.23) + padding) + "px");
    } else {
        $("#viewBody").css("padding-left", "0px");
        $("#viewBody").css("padding-right", "0px");
        $("#bcBody").css("left", ($("#viewBody").width() * 0.23) + "px");
    }
    $("#bcBody").css("height", ($("#viewBody").width() * 0.12) + "px");
    $("#bcBody").css("top", ((($("#viewBody").height() - $("#navBar").height()) * 0.45) + $("#navBar").height() - ($("#bcBody").height() / 2) + "px"));
    $("#bcTape").css("left", ($("#bcBody")[0].getBoundingClientRect().left + ($("#bcBody").width() * 0.13)) + "px");
    $("#bcTape").css("width", ($("#bcBody").width() * 0.07) + "px");
});

var screenRatio = $(window).width() / $(window).height();
if (screenRatio > 1.33) {
    var padding = ($(window).width() - ($(window).height() * 1.33)) / 2;
    $("#viewBody").css("padding-left", padding + "px");
    $("#viewBody").css("padding-right", padding + "px");
    $("#bcBody").css("left", (($("#viewBody").width() * 0.23) + padding) + "px");
} else {
    $("#viewBody").css("padding-left", "0px");
    $("#viewBody").css("padding-right", "0px");
    $("#bcBody").css("left", ($("#viewBody").width() * 0.23) + "px");
}

$(".sectionHeader").each(function() {
    var divRatio = $(this).width() / $(this).height();
    $(this).find(".areaFit").css("font-size", (1 + (divRatio / 2)) * 2 + "vh");
});

$(".sectionBody").each(function() {
    var divRatio = $(this).width() / $(this).height();
    $(this).find(".areaFit").css("font-size", (1 + (divRatio / 2)) * 1.4 + "vh");
});

$("#bcBody").css("height", ($("#viewBody").width() * 0.12) + "px");
$("#bcBody").css("top", ((($("#viewBody").height() - $("#navBar").height()) * 0.45) + $("#navBar").height() - ($("#bcBody").height() / 2) + "px"));
$("#bcTape").css("top", $("#chunkThree")[0].getBoundingClientRect().top + "px");
$("#bcTape").css("left", ($("#bcBody")[0].getBoundingClientRect().left + ($("#bcBody").width() * 0.13)) + "px");
$("#bcTape").css("width", ($("#bcBody").width() * 0.07) + "px");