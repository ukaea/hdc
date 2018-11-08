%EXAMPLE_INTERFACE Example MATLAB class wrapper to an underlying C++ class
classdef HDC < handle
    properties (SetAccess = private, Hidden = true)
        objectHandle; % Handle to the underlying C++ class instance
    end
    methods
        %% Constructor - Create a new C++ class instance
        function this = HDC(varargin)
            if (length(varargin) > 0)
                for i = 1:length(varargin)
                    data = varargin{i};
                    if (class(data) == "string")
                        varargin(i) = {char(data)};
                    end
                end
                if (class(varargin{1}) == "char")
                    if (varargin{1} == "__noinit")
                        return;
                    end
                end
            end
            this.objectHandle = hdc_mex('new', varargin{:});
        end

        %% Destructor - Destroy the C++ class instance
        function delete(this)
            hdc_mex('delete', this.objectHandle);
        end

        function varargout = set_data(this, data)
            inp = {data};
            if (class(data) == "string")
                data = char(data);
            end
            [varargout{1:nargout}] = hdc_mex('set_data', this.objectHandle, inp{:});
        end

        function varargout = get_data(this)
            inp = {};
            [varargout{1:nargout}] = hdc_mex('get_data', this.objectHandle, inp{:});
        end

        function varargout = add_child(this, path, child)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('add_child', this.objectHandle, inp{:});
        end

        function varargout = set_child(this, path, child)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('set_child', this.objectHandle, inp{:});
        end

        function varargout = set(this, path, something)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path};
            if (class(something) == "HDC")
                this.add_child(path, something);
            else
                ch = HDC(1,hdc_mex('get_or_create', this.objectHandle, path));
                ch.set_data(something);
                ch.dump()
            end
        end

        function child = get_child(this, path)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path};
            child = HDC(1,hdc_mex('get_child', this.objectHandle, inp{:}));
        end

        function varargout = append(this, child)
            inp = {child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('append', this.objectHandle, inp{:});
        end

        function varargout = insert(this, index, child)
            inp = {index, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('insert', this.objectHandle, inp{:});
        end

        function varargout = dump(this)
            [varargout{1:nargout}] = hdc_mex('dump', this.objectHandle);
        end

        function varargout = as_hdc_t(this)
            % Create MATLAB struct
            [varargout{1:nargout}] = hdc_mex('as_hdc_t', this.objectHandle)
        end
        function result = at(this, path)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path};
            ch = HDC(1,hdc_mex('get_child',this.objectHandle, inp{:}));
            t = hdc_mex('type',ch.objectHandle)
            if (t == "double" || t == "single" || t == "int8"   || t == "int16"  || t == "int32" || t == "int64" ||  ...
                t == "uint8"  || t == "uint16" || t == "uint32" || t == "uint64" || t == "string")
                result = hdc_mex('get_data',ch.objectHandle);
            else
                result = ch;
            end
        end
        function delete_child(this, path)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path};
            hdc_mex('delete_child',this.objectHandle,inp{:});
        end
    end
    methods(Static)
        function this = load_json(path,data_path)
            this = HDC("__noinit")
            inp = {char(path),char(data_path)};
            this.set_handle(hdc_mex('load_json',inp{:}));
        end
        function this = load_hdf5(path,data_path)
            this = HDC("__noinit")
            inp = {char(path),char(data_path)};
            this.set_handle(hdc_mex('load_hdf5',inp{:}));
        end
        function destroy()
            hdc_mex('destroy');
        end

        function init(varargin)
            if (length(varargin) >= 0)
                for i = 1:length(varargin)
                    data = varargin{i};
                    if (class(data) == "string")
                        varargin(i) = {char(data)};
                    end
                end
            end
            hdc_mex('init',varargin{:});
        end
    end
end
