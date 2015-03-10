local endtime = curt+6
local midtime = curt+3
local alphaf = false

function update(self)	
--	if curt > endtime then
--		bazinga.set_scene('scenes/cena01.scene')
--	end
--
--	if curt > midtime then
--		self.img = "assets/images/ccsl.png"
--		alphaf = true
--	end
--
--	if alphaf then
--		self.alpha = math.sin((endtime-curt)/3*math.pi)
--	else
--		self.alpha = math.sin((midtime-curt)/3*math.pi)
--	end
--
--	local s = (bazinga.screen_h/2)/self.img_h
--
--	bazinga.set_camera (self.x+self.img_w/2, self.y+self.img_h/2)
--	bazinga.set_zoom (s, s)
--
	return self
end