%EXAMPLE_INTERFACE Example MATLAB class wrapper to an underlying C++ class
classdef HDC < handle
    properties (SetAccess = private, Hidden = true)
        objectHandle; % Handle to the underlying C++ class instance
    end
    methods
        %% Constructor - Create a new C++ class instance
        function this = HDC(varargin)
            for i = 1:length(varargin)
                data = varargin{i};
                if (class(data) == "string")
                    varargin(i) = {char(data)};
                end
            end
            this.objectHandle = hdc_mex('new', varargin{:});
        end

        %% Destructor - Destroy the C++ class instance
        function delete(this)
            hdc_mex('delete', this.objectHandle);
            % disp('do not destroy HDC >:-(')
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

        function varargout = add(this, path, child)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('add', this.objectHandle, inp{:});
        end

        function varargout = set(this, path, child)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path, child.objectHandle};
            [varargout{1:nargout}] = hdc_mex('set', this.objectHandle, inp{:});
        end

        function child = get(this, path)
            if (class(path) == "string")
                path = char(path);
            end
            inp = {path};
            child = HDC(1,hdc_mex('get', this.objectHandle, inp{:}));
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
            sm.storage = hdc_mex('get_storage', this.objectHandle)
            sm.uuid = char(hdc_mex('get_uuid', this.objectHandle))
            [varargout{1:nargout}] = sm
        end
    end
    methods(Static)
        function res = load_json(path,data_path)
//             inp = {char(path),char(data_path)};
//             [varargout{1:nargout}] = hdc_mex('load_json', inp{:});
            disp("load_json(): Not implemented yet");
        end

        function destroy()
            disp("destroy(): Not implemented yet");
        end

        function init()
            disp("init(): Not implemented yet");
        end
    end
end
