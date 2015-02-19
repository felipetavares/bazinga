function main_keypress (ctx, key)
  if key == 'up' then
    ctx.up = 1
  end

  if key == 'down' then
    ctx.down = 1
  end

  if key == 'left' then
    ctx.left = 1
  end

  if key == 'right' then
    ctx.right = 1
  end

  if key == 'space' then
    ctx.space = 1
  end

  return ctx
end

function main_keyunpress (ctx, key)
  if key == 'up' then
    ctx.up = 0
  end

  if key == 'down' then
    ctx.down = 0
  end

  if key == 'left' then
    ctx.left = 0
  end

  if key == 'right' then
    ctx.right = 0
  end

  if key == 'space' then
    ctx.space = 0
  end

  return ctx
end
