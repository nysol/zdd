require "rubygems"
require "nysol/zdd_so"

class String	
	alias_method :plus ,:+
	def +(other)
		if String == other.class then
			return ( self.plus(other) )   
		else
			return (ZDD.itemset(self) + other)
		end
	end

	#宣言してからzdd生成(引数がmoduleだった場合"+"とする)
	def v(val=nil,to="bottom")		
		if ( val.kind_of? Module) then
			ZDD.symbol(self,nil,"bottom")
			base = ZDD.itemset(self)
			return base + val
		else 
			ZDD.symbol(self,val,to)
			return ZDD.itemset(self)
		end
	end
end

class Integer
	def v
		ZDD.constant(self)
	end
end
