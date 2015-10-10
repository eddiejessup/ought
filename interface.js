/*global $, jQuery, d3, graphApi, graphDrawForce */

$(document).ready(function() {
  var defaultPauseTime = 100;
  var interval;
  var graphDraw = graphDrawForce;

  function startIteration() {
    var pauseTime = $('#step').val() || defaultPauseTime;
    console.log(pauseTime);
    interval = setInterval(function() {
      graphApi.iterate();
      graphDraw.render();
    }, pauseTime);
  }

  function toggleIteration() {
    if (isPlaying) {
      clearInterval(interval);
      isPlaying = false;
      $('#pause').text('Play');
    } else {
      startIteration();
      isPlaying = true;
      $('#pause').text('Pause');
    }
  }

  function createGraph() {
    var x = $('#x').val();
    var y = $('#y').val();
    graphApi.create(x, y);
    graphDraw.clear();
    graphDraw.render();
  }

  $('#pause').on('click', toggleIteration);
  $('#reset').on('click', createGraph);
  $('#create').on('click', createGraph);

  var isPlaying = false;
});
