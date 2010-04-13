function OnClicked(x, y, b)
{
  this.Text = 'OnClicked called!(' + x + ',' + y + ',' + b + ')';
  this.TextColor = 'green';
  this.Grab();
}

function OnUnclicked(x, y, b)
{
  this.Text = 'OnUnclicked called!(' + x + ',' + y + ',' + b + ')';
  this.TextColor = 'blue';
  this.Ungrab();
}

function OnMoved(x, y)
{
  this.Text = 'OnMoved called!(' + x + ',' + y + ')';
  this.TextColor = 'red';
}

function main()
{
  NGL_OUT('nuiRight:' + nuiRight);
  var label = new nuiLabel('FromJS');
  NGL_OUT('label class:'+label.GetObjectClass());
  window.AddChild(label);
  NGL_OUT(window.GetObjectClass());
  var test = label.test;
  label.test = 'testval';
  var res = label.Text;
  NGL_OUT(res);
  label.TextColor = 'red';
  label.Position = nuiRight;
  label.Clicked = OnClicked;
  label.Unclicked = OnUnclicked;
  label.MovedMouse = OnMoved;
}
