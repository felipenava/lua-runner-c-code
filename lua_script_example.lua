EXPORTED_C_FUNCTIONS = {}
EXPORTED_C_FUNCTIONS.kFileSize = "1" 

function GetFileSize(path)
   if not HaveCFunction(EXPORTED_C_FUNCTIONS.kFileSize) then
      return nil
   end
   return CallCFunction(EXPORTED_C_FUNCTIONS.kFileSize, path)
end

function Process(file_path)
	ret = GetFileSize(file_path)
	return ret
end