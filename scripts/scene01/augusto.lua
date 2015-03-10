dialogs = {
	'Agora você vai ver. Vai pedir ajuda a quem? Somos melhores do que você. Se toca!',
	'Grita! Vai, grita que quero ver! Eu vou te perseguir até no inferno!! Se você contar para alguém...',
	'Você não reage porque é fraco! Pessoas fracas choram, pode chorar. Você nunca será ninguém, nunca será melhor do que eu! Nunca!',
	'Não consegue ficar mais em pé, né? Você é um verme, nunca passará disso, verme! Agora rasteje!',
}
dialogNum = 1
dialog = -1
accept = true
spacebarAnimationID = bazinga.search_object('spacebar indicator')
hitt = 0

function update(self)
	if dialog ~= -1 then
		accept = bazinga.ended_dialog(dialog)
	end

	bazinga.hide_object(spacebarAnimationID, not accept)

	if self.anim ~= '' and self.anim_frame == 8 then
		hitt = curt + 0.1 -- Hit during the next 0.2s
end

	if self.anim ~= '' and self.anim_frame == 14 then
		dialog = bazinga.new_dialog(dialogs[dialogNum], 'assets/images/augusto-face.png', 'sb')

		dialogNum = dialogNum+1

		self.anim = ""
	end

	if ctx_main.space == 1 and accept and dialogNum <= #dialogs then
		if dialog ~= -1 then
			bazinga.del_dialog(dialog)
		end
		self.anim = 'assets/animations/augusto-slap.anim';
	end


	if curt < hitt then
		bazinga.set_camera(self.x + 3*math.cos(curt*100), self.y + 3*math.sin(curt*100))
	else
		bazinga.set_camera(self.x, self.y)
	end

	return self
end