function collision_begin (a, b)
	bazinga.del_object(b.id)

	return true
end

bazinga.set_reorder(true)
bazinga.new_font('assets/fonts/texgyrecursor-regular.otf', 'sb')
bazinga.set_font_size('sb', 16)
bazinga.set_font_color('sb', 0, 0, 0, 1)
bazinga.set_zoom(2, 2)
bazinga.set_background_color(0.5, 0.5, 0.5)