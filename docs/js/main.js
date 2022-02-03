$("#viewBody").scroll(function() {
    var offset = 0;
    var animMargin = $("#viewBody").height() - ($("#viewBody").height() * 0.1);
    var chunk4Top = $("#chunkFour")[0].getBoundingClientRect().top;
    var chunk5Top = $("#chunkFive")[0].getBoundingClientRect().top;
    if (chunk4Top < animMargin) {
        offset = ((1 - (chunk4Top / animMargin)) * ($("#bcBody").height() / 50));
    }
    if (chunk5Top < animMargin) {
        $("#bcBody").css("opacity", ((chunk5Top / animMargin) - 0.5));
    } else {
        $("#bcBody").css("opacity", "1");
    }

    if ($("#viewBody").scrollTop() > $("#viewBody").height() * 0.5 && $("#viewBody").scrollTop() < $("#viewBody").height() * 1.5) {
        $(".chunk2layer").show();
    } else {
        $(".chunk2layer").hide();
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
    $("#bcTape2").css("top", $("#chunkThree")[0].getBoundingClientRect().top + "px");

});

$(window).resize(function() {

    sizeAndPosition();

});

sizeAndPosition();

function sizeAndPosition() {

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

    $("#bcTape2").css("top", $("#chunkThree")[0].getBoundingClientRect().top + "px");
    $("#bcTape2").css("left", ($("#bcBody")[0].getBoundingClientRect().left + "px"));
    $("#bcTape2").css("width", ($("#bcBody").width() * 0.07) + "px");

    $("#ydim").css("width", $("#bcBody").css("width"));
    $("#ydim").css("top", $("#bcBody")[0].getBoundingClientRect().top - ($("#ydim").height() * 1.2) + "px");
    $("#ydim").css("left", $("#bcBody").css("left"));

    $("#xdim").css("height", $("#bcBody").css("height"));
    $("#xdim").css("left", $("#bcBody")[0].getBoundingClientRect().left - ($("#xdim").width() * 1.2) + "px");
    $("#xdim").css("top", $("#bcBody").css("top"));

    $("#cr2032").css("height", $("#bcBody").css("height"));
    $("#cr2032").css("left", $("#bcBody")[0].getBoundingClientRect().left + $("#bcBody").width() - ($("#cr2032").width() * .5) + "px");
    $("#cr2032").css("top", $("#bcBody")[0].getBoundingClientRect().top + ($("#cr2032").height() * .5) + "px");

}