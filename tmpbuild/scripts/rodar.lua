function update (self)
	if ctx_main.up == 1 then
		self.y = self.y-1
	end

	if ctx_main.down == 1 then
		self.y = self.y+1
	end

	if ctx_main.right == 1 then
		self.x = self.x-1
	end

	if ctx_main.left == 1 then
		self.x = self.x+1
	end

	return self
end
