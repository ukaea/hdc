%EXAMPLE_INTERFACE Example MATLAB class wrapper to an underlying C++ class
classdef HDC < handle %& matlab.mixin.CustomDisplay
    properties (SetAccess = private, Hidden = true)
        objectHandle; % Handle to the underlying C++ class instance
    end
    methods
        %% Constructor - Create a new C++ class instance
        function this = HDC(varargin)
            if (length(varargin) > 0)
                for i = 1:length(varargin)
                    data = varargin{i};
                    if (class(data) == "string"); varargin(i) = {char(data)}; end
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
            if (class(data) == "string"); data = char(data); end
            inp = {data};
            [varargout{1:nargout}] = hdc_mex('set_data', this.objectHandle, inp{:});
        end

        function varargout = get_data(this)
            inp = {};
            [varargout{1:nargout}] = hdc_mex('get_data', this.objectHandle, inp{:});
        end

        function varargout = add_child(this, path, child)
            if (class(path) == "string"); path = char(path); end
            inp = {path, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('add_child', this.objectHandle, inp{:});
        end

        function varargout = set_child(this, path, child)
            if (class(path) == "string"); path = char(path); end
            inp = {path, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('set_child', this.objectHandle, inp{:});
        end

        function varargout = set(this, path, something)
            if (class(path) == "string"); path = char(path); end
            inp = {path};
            if (class(something) == "HDC")
                if (this.exists(path))
                    this.set_child(path, something);
                else
                    this.add_child(path, something);
                end
            else
                ch = HDC(1,hdc_mex('get_or_create', this.objectHandle, path));
                ch.set_data(something);
            end
        end

        function child = get_child(this, path)
            if (class(path) == "string"); path = char(path); end
            inp = {path};
            child = HDC(1,hdc_mex('get_child', this.objectHandle, inp{:}));
        end

        function result = shape(this)
            result = hdc_mex('shape', this.objectHandle);
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

        function varargout = dumps(this)
            [varargout{1:nargout}] = hdc_mex('dumps', this.objectHandle);
        end

        function varargout = as_hdc_t(this)
            % Create MATLAB struct
            [varargout{1:nargout}] = hdc_mex('as_hdc_t', this.objectHandle);
        end

        function result = at(this, path)
            if (class(path) == "string"); path = char(path); end
            inp = {path};
            ch = HDC(1,hdc_mex('get_child',this.objectHandle, inp{:}));
            t = hdc_mex('type',ch.objectHandle);
            if (t == "double" || t == "single" || t == "int8"   || t == "int16"  || t == "int32" || t == "int64" ||  ...
                t == "uint8"  || t == "uint16" || t == "uint32" || t == "uint64" || t == "string")
                result = hdc_mex('get_data',ch.objectHandle);
            else
                result = ch;
            end
        end

        function delete_child(this, path)
            if (class(path) == "string"); path = char(path); end
            inp = {path};
            hdc_mex('delete_child',this.objectHandle,inp{:});
        end

        function result = exists(this, path)
            if (class(path) == "string"); path = char(path); end
            inp = {path};
            result = hdc_mex('exists',this.objectHandle,inp{:});
        end

        function result = keys(this)
            result = hdc_mex('keys',this.objectHandle);
        end

        function result = copy(this)
            result = HDC(1,hdc_mex('copy',this.objectHandle));
        end

        function save(this,varargin)
            uri = varargin{1};
            if (class(uri) == "string"); uri = char(uri); end
            inp = {uri};
            if (length(varargin) >= 2)
                data_path = varargin{2};
                if (class(data_path) == "string"); data_path = char(data_path); end
                % This does not work, we have to do some work around
                % inp = {uri,data_path};
                inp = join([inp{1},"|",data_path],"");
            end
            hdc_mex('save', this.objectHandle, inp{:});
        end
    end
    methods(Static)

        function result = load(varargin)
            uri = varargin{1};
            if (class(uri) == "string"); uri = char(uri); end
            inp = {uri};
            if (length(varargin) >= 2)
                data_path = varargin{2};
                if (class(data_path) == "string"); data_path = char(data_path); end
                inp = {uri,data_path};
            end
            result = HDC(1,hdc_mex('load', inp{:}));
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
