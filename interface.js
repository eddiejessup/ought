/*global $, jQuery, graphInteract */

$(document).ready(function() {
  $('#pause').on('click', graphInteract.toggleIteration);
  $('#create').on('click', graphInteract.createGraph);
});
