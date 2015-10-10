/*global $, jQuery*/

var graphApi = (function () {
  var domain = "http://localhost:5000";
  var crossDomain = true;

  function getGraph(context, callback) {
    $.ajax({
      context: context,
      method: "GET",
      url: domain,
      dataType: "json",
      crossDomain: crossDomain
    }).done(callback);
  }

  function iterate() {
    $.ajax({
      method: "GET",
      url: domain + '/iterate',
      dataType: "text",
      crossDomain: crossDomain,
    }).done(function (data) { console.log(data); });
  }

  function create(x, y) {
    $.ajax({
      method: "GET",
      url: domain + '/create/' + x + ',' + y,
      dataType: "text",
      crossDomain: crossDomain
    }).done(function (data) { console.log(data); });
  }

  return {
    create: create,
    iterate: iterate,
    getGraph: getGraph,
  };
}());
